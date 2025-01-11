#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define COUNT 400

char** patterns;

bool str_compare(char* a, char* b, int len) {
	for(int i = 0; i < len; i++) {
		if(a[i] != b[i]) return false;
	}
	return true;
}

typedef struct {
	char* key;
	long value;
} Cache;

Cache* cache = NULL;

long CountPossibleTovels(char* line) {
	long value = shget(cache, line);
	if(value) return value;

	if(*line == 0) return 1;

	long possible = 0;

	for(int i = 0; i < arrlen(patterns); i++) {
		if(str_compare(line, patterns[i], strlen(patterns[i])))
			possible += CountPossibleTovels(line+strlen(patterns[i]));
	}

	shput(cache, line, possible);
	return possible;
}


long SolveFile(char* fileName) {
	long validTovels = 0;
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	fseek(f, 0, SEEK_SET);
	char* line = NULL;
	size_t lineLen;
	getline(&line, &lineLen, f);
	char* token = strtok(line, ", ");
	while(token) {
		char* pattern = calloc((strlen(token)+1), sizeof(char));
		strcpy(pattern, token);
		arrput(patterns, pattern);
		token = strtok(NULL, ", \n");
	}

	getline(&line, &lineLen, f);

	for(int i = 0; i < COUNT; i++) {
		getline(&line, &lineLen, f);
		line[strlen(line)-1] = 0;
		validTovels += CountPossibleTovels(line);
	}
	free(line);

	fclose(f);
	return validTovels;
}

int main() {
	long validTovels = SolveFile("input.txt");

	printf("Part 2: %ld\n", validTovels);
	return 0;
}
