#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUCKET_AMOUNT 255


int getLines(FILE* file) {
	char ch;
	int linesCount = 0;
	while((ch = fgetc(file)) != EOF) {
		if(ch == '\n')
			linesCount++;
	}

	return linesCount;
}

int parseFile(char* fileName, int** arr1, int** arr2) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}
	int linesCount = getLines(f);
	*arr1 = (int*)malloc(sizeof(int)*linesCount);
	*arr2 = (int*)malloc(sizeof(int)*linesCount);

	fseek(f, 0, SEEK_SET);
	char* line = NULL;
	size_t lineLen;
	for (int i = 0; getline(&line, &lineLen, f) != -1; i++) {
		char* token = strtok(line, " ");
		(*arr1)[i] = atoi(token);
		token = strtok(NULL, " ");
		int a = atoi(token);
		(*arr2)[i] = a;
	}
	free(line);

	fclose(f);
	return linesCount;
}

int intCompare(const void *a, const void *b) {
  int *valA = a;
  int *valB = b;
  return *valA - *valB;
}

typedef struct number Number;
struct number {
	int num;
	int count;
	struct number* next;
};

int hash(int num) {
	return (num ^ 0b01010101010101010101010101010101) & BUCKET_AMOUNT;
}

typedef struct {
	Number* nums[BUCKET_AMOUNT+1];
} Dict;

Number* getNum(Dict* dict, int num) {
	int index = hash(num);
	Number* number = dict->nums[index];
	while(number != NULL) {
		if(number->num == num) return number;	
		number = number->next;
	}
	
	return NULL;
}

void addNum(Dict* dict, int num) {
	int index = hash(num);
	Number* new = (Number*)calloc(1, sizeof(Number));
	new->num = num;
	new->count = 1;
	Number* number = dict->nums[index];
	if(number == NULL) {
		dict->nums[index] = new;
		return;
	}
	while(number->next != NULL) {
		number = number->next;
	}
	number->next = new;
}

int main() {
	int* list1;
	int* list2;
	int count = parseFile("input.txt", &list1, &list2);

	qsort(list1, count, sizeof(int), intCompare);
	qsort(list2, count, sizeof(int), intCompare);
	int sum1 = 0;
	for (int i = 0; i < count; i++) {
		sum1 += abs(list1[i] - list2[i]);
	}
	printf("Part 1: %d\n", sum1);

	Dict dict = {0};
	for(int i = 0; i < count; i++) {
		Number* number;
		if(number = getNum(&dict, list2[i])) {
			number->count++;
			continue;
		}
		addNum(&dict, list2[i]);
	}

	int sum2 = 0;
	for(int i = 0; i < count; i++) {
	 	Number* number;
		if(number = getNum(&dict, list1[i])) {
			sum2 += list1[i] * number->count;
		}
	}
	
	printf("Part 2: %d\n", sum2);

	free(list1);
	free(list2);
	return 0;
}


