#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define RULES_COUNT 1176
#define ARRAY_LENGTH 6
#define BUCKET_AMOUNT 255

/* Array */
typedef struct {
	int* nums;
	int count;
	int allocated;
} IntArray;

void InitArray(IntArray* array) {
	array->count = 0;
	array->allocated = ARRAY_LENGTH;
	array->nums = (int*)calloc(ARRAY_LENGTH, sizeof(int));
}

void AddNum(IntArray* array, int num) {
	if(array->count == array->allocated) {
		array->nums = (int*)realloc(array->nums, sizeof(int)*(array->allocated*=2));
	}
	array->nums[array->count++] = num;
}

void EmptyArray(IntArray* array) {
	array->count = 0;
}

/* Dictionary */
typedef struct rule Rule;
struct rule {
	int first;
	IntArray seconds;
	struct rule* next;
};

int hash(int num) {
	return (num ^ 0b01010101010101010101010101010101) & BUCKET_AMOUNT;
}

typedef struct {
	Rule* buckets[BUCKET_AMOUNT+1];
} Dict;
Dict rules = {0};

Rule* GetRule(Dict* dict, int first) {
	int index = hash(first);
	Rule* rule = dict->buckets[index];
	while(rule != NULL) {
		if(rule->first == first) return rule;	
		rule = rule->next;
	}
	
	return NULL;
}

void AddRule(Dict* dict, int first, int second) {
	Rule* newRule = (Rule*)calloc(1, sizeof(Rule));
	newRule->first = first;
	InitArray(&newRule->seconds);
	AddNum(&newRule->seconds, second);

	int index = hash(first);
	Rule* rule = dict->buckets[index];
	if(rule == NULL) {
		dict->buckets[index] = newRule;
		return;
	}
	while(rule->next != NULL) {
		rule = rule->next;
	}
	rule->next = newRule;
}

/* Logic */
void ReadRules(FILE* f) {
	char* line = NULL;
	size_t lineLen;
	for (int i = 0; i < RULES_COUNT; i++) {
	 	getline(&line, &lineLen, f);
		char* token = strtok(line, "|");
		int first = atoi(token);
		token = strtok(NULL, " ");
		int second = atoi(token);
		Rule* rule = GetRule(&rules, first);
		if(rule == NULL) AddRule(&rules, first, second);
		else AddNum(&rule->seconds, second);
	}
	free(line);
}

int RulesComparator(const void *a, const void* b) {
	int* first = a;
	int* second = b;
	Rule* rule = GetRule(&rules, *first);
	assert(rule != NULL);
	for(int i = 0; i < rule->seconds.count; i++) {
		if(rule->seconds.nums[i] == *second) return 0;
	}
	return 1;
}

bool FollowsRules(int first, int second) {
	Rule* rule = GetRule(&rules, first);
	assert(rule != NULL);
	for(int i = 0; i < rule->seconds.count; i++) {
		if(rule->seconds.nums[i] == second) return true;
	}
	return false;
}

bool ValidUpdate(IntArray* array) {
	for(int i = 0; i < array->count-1; i++) {
		for(int j = i+1; j < array->count; j++) {
			if(!FollowsRules(array->nums[i], array->nums[j])) return false;
		}
	}
	return true;
}

void SolveFile(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}
	ReadRules(f);

	int validSum = 0;
	int correctedSum = 0;
	char* line = NULL;
	size_t lineLen;
	IntArray array;
	InitArray(&array);
	// might need to skip line
	getline(&line, &lineLen, f); //skip empty line
	for (int i = 0; getline(&line, &lineLen, f) != -1; i++) {
		char* token = strtok(line, ",");
		while(token != NULL) {
			AddNum(&array, atoi(token));
			token = strtok(NULL, ",");
		}
		if(ValidUpdate(&array)) {
			validSum += array.nums[(array.count-1)/2];
		}
		else {
			qsort(array.nums, array.count, sizeof(int), RulesComparator);
			correctedSum += array.nums[(array.count-1)/2];
		}
		EmptyArray(&array);
	}
	free(line);

	fclose(f);

	printf("Part 1: %d\n", validSum);
	printf("Part 2: %d\n", correctedSum);
}

int main() {
	
	SolveFile("input.txt");
	
	return 0;
}
