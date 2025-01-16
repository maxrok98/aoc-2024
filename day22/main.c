#include <stdio.h>
#include <stdlib.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define COUNT  2462

long nums[COUNT] = {0};

void ParseFile(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}
	fseek(f, 0, SEEK_SET);

	char* line = NULL;
	size_t lineLen;
	for (int i = 0; i < COUNT; i++) {
	 	getline(&line, &lineLen, f);
		nums[i] = atoi(line);
	}
	free(line);

	fclose(f);
}

long Evolve(long num, int times) {
	long a = num;
	for(int i = 0; i < times; i++) {
		a = (a ^ (a * 64)) % 16777216;
		a = (a ^ (a / 32)) % 16777216;
		a = (a ^ (a * 2048)) % 16777216;
	}
	return a;
}

typedef struct {
	char first;
	char second;
	char third;
	char fourth;
} Sequence;

typedef struct {
	Sequence key;
	char value;
} Dict;

Dict* dict[COUNT] = {0};

void GenerateSequences(long num, int dictNum) {
	num = Evolve(num, 1);
	char first = num % 10;
	num = Evolve(num, 1);
	char second = num % 10;
	num = Evolve(num, 1);
	char third = num % 10;
	num = Evolve(num, 1);
	char fourth = num % 10;
	for(int i = 4; i < 2000; i++) {
		num = Evolve(num, 1);
		char next = num % 10;
		Sequence s = { .first = second - first, .second = third - second, .third = fourth - third, .fourth = next - fourth };
		if(!hmget(dict[dictNum], s)) hmput(dict[dictNum], s, next);

		first = second;
		second = third;
		third = fourth;	
		fourth = next;
	}
}

Dict* uniqueSeq = NULL;

long CountBananas(Sequence s) {
	long a = 0;
	for(int i = 0; i < COUNT; i++) {
		a += hmget(dict[i], s);
	}
	return a;
}

int main() {
	ParseFile("input.txt");
	
	long sum = 0;
	for(int i = 0; i < COUNT; i++) {
		sum += Evolve(nums[i], 2000);
		GenerateSequences(nums[i], i);
	}
	printf("Part 1: %ld\n", sum);
	
	for(int i = 0; i < COUNT; i++) {
		for(int j = 0; j < hmlen(dict[i]); j++) {
			if(!hmget(uniqueSeq, dict[i][j].key)) hmput(uniqueSeq, dict[i][j].key, 1);
		}
	}
	long biggest = 0;
	for(int i = 0; i < hmlen(uniqueSeq); i++) {
		long a = CountBananas(uniqueSeq[i].key);
		if(a > biggest) biggest = a;
	}

	printf("Part 2: %ld\n", biggest);

	return 0;
}
