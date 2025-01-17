#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define COUNT (4000 / 8)

typedef struct {
	int scheme[5];
} Scheme;

char schematic[7][5];

Scheme* keys = NULL;
Scheme* locks = NULL;

Scheme SchematicToScheme(char s[7][5]) {
	Scheme ns = {0};
	for(int j = 0; j < 5; j++) {
		int count = 0;
		for(int i = 0; i < 7; i++) {
			if(s[i][j] == '#') count++;
		}
		ns.scheme[j] = --count;
	}

	return ns;
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
	for (int i = 0; i < COUNT; i++) {
	 	getline(&line, &lineLen, f);
		memcpy(schematic[0], line, 5);
	 	getline(&line, &lineLen, f);
		memcpy(schematic[1], line, 5);
	 	getline(&line, &lineLen, f);
		memcpy(schematic[2], line, 5);
	 	getline(&line, &lineLen, f);
		memcpy(schematic[3], line, 5);
	 	getline(&line, &lineLen, f);
		memcpy(schematic[4], line, 5);
	 	getline(&line, &lineLen, f);
		memcpy(schematic[5], line, 5);
	 	getline(&line, &lineLen, f);
		memcpy(schematic[6], line, 5);

		Scheme s = SchematicToScheme(schematic);
		if(schematic[0][0] == '#') arrput(locks, s);
		else arrput(keys, s);

	 	getline(&line, &lineLen, f);
	}
	free(line);

	fclose(f);
}

bool Fits(Scheme key, Scheme lock) {
	for(int i = 0; i < 5; i++) {
		if(key.scheme[i] + lock.scheme[i] >= 6) return false;
	}
	return true;
}

int main() {
	ParseFile("input.txt");
	int sum = 0;
	for(int i = 0; i < arrlen(keys); i++) {
		for(int j = 0; j < arrlen(locks); j++) {
			if(Fits(keys[i], locks[j])) sum++;
		}
	}

	printf("Part 1: %d\n", sum);

	return 0;
}
