#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define FILE_LINES 850

long* equations[FILE_LINES] = {0};

void printArray(int* array) {
	for(int i = 0; i < arrlen(array); i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

void ParseFile(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	fseek(f, 0, SEEK_SET);
	char* line = NULL;
	size_t lineLen;
	for (int i = 0; getline(&line, &lineLen, f) != -1; i++) {
		char* token = strtok(line, ":");
		while(token != NULL) {
			arrput(equations[i], atol(token));
			token = strtok(NULL, " ");
		}
	}
	free(line);

	fclose(f);
}

bool ValidEquation1(long* equation, long result, int index) {
	if(index == arrlen(equation)-1) {
		if(result + equation[index] == equation[0]) return true;
		if(result * equation[index] == equation[0]) return true;
		return false;
	}
	if(ValidEquation1(equation, result + equation[index], index+1)) return true;
	if(ValidEquation1(equation, result * equation[index], index+1)) return true;
	return false;
}

long Pow(long x,long n) {
    long number = 1;
    for (int i = 0; i < n; ++i)
        number *= x;
    return number;
}

long Concat(long a, long b) {
	for(long i = 0; b / Pow(10,i) != 0 ;i++) {
		a *= 10;
	}
	return a + b;
}

bool ValidEquation2(long* equation, long result, int index) {
	if(index == arrlen(equation)-1) {
		if(result + equation[index] == equation[0]) return true;
		if(result * equation[index] == equation[0]) return true;
		if(Concat(result, equation[index]) == equation[0]) return true;
		return false;
	}
	if(ValidEquation2(equation, result + equation[index], index+1)) return true;
	if(ValidEquation2(equation, result * equation[index], index+1)) return true;
	if(ValidEquation2(equation, Concat(result, equation[index]), index+1)) return true;
	return false;
}

int main() {
	ParseFile("input.txt");

	long sum = 0;
	for(int i = 0; i < FILE_LINES; i++) {
		if(ValidEquation1(equations[i], equations[i][1], 2)) sum+=equations[i][0];
	}
	printf("Part 1: %ld\n", sum);

	sum = 0;
	for(int i = 0; i < FILE_LINES; i++) {
		if(ValidEquation2(equations[i], equations[i][1], 2)) sum+=equations[i][0];
	}
	printf("Part 2: %ld\n", sum);

	return 0;
}

