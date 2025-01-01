#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

long* ParseFile(char* fileName) {
	long* arr = NULL;
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	char* line = NULL;
	size_t lineLen;
	getline(&line, &lineLen, f);

	char* token = strtok(line, " ");
	while(token != NULL) {
		arrput(arr, atol(token));
		token = strtok(NULL, " ");
	}
	free(line);
	fclose(f);
	return arr;
}

int CountDigits(long a) {
	int i;
	for(i = 0; a > 0; i++) {
		a /= 10;
	}
	return i;
}

bool EvenDigits(long a) {
	return !(CountDigits(a) % 2);
}

long Pow(long x,long n) {
    long number = 1;
    for (int i = 0; i < n; ++i)
        number *= x;
    return number;
}

void Split(long num, long* n1, long* n2) {
	int numLen = CountDigits(num);
	for(int i = 0; i < (numLen/2); i++) {
		//*n2 *= 10;
		*n2 += (num % 10) * Pow(10, i);
		num /= 10;
	}
	*n1 = num;
}

void Blink(long** arr) {
	int len = arrlen(*arr);
	for(int i = 0; i < len; i++) {
		if((*arr)[i] == 0) (*arr)[i] = 1;
		else if(EvenDigits((*arr)[i])) {
			long a = 0, b = 0;
			Split((*arr)[i], &a, &b);
			(*arr)[i] = a;
			arrput(*arr, b);
		}
		else {
			(*arr)[i] *= 2024;
		}
	}
}

typedef struct {
	long num;
	int blinkN;
} Expandable;

typedef struct {
	Expandable key;
	long value;
} Cache;

Cache* cache = NULL;
long Blinking(long num, long currentBlink, long totalBlinks) {
	if(currentBlink == totalBlinks) return 1;

	Expandable exp = { .num = num, .blinkN = currentBlink };
	long expandsTo = hmget(cache, exp);
	if(expandsTo != -1) return expandsTo;
	long* tempArr = NULL;
	arrput(tempArr, num);
	Blink(&tempArr);
	long sum = 0;
	for(int i = 0; i < arrlen(tempArr); i++) {
		sum += Blinking(tempArr[i], currentBlink+1, totalBlinks);
	}
	hmput(cache, exp, sum);
	arrfree(tempArr);
	return sum;
}

int main() {
	long* arr = ParseFile("input.txt");

	for(int i = 0; i < 25; i++) {
		Blink(&arr);
	}

	printf("Part 1: %ld\n", arrlen(arr));
	arrfree(arr);

	hmdefault(cache, -1);
	arr = ParseFile("input.txt");
	long sum = 0;
	for(int i = 0; i < arrlen(arr); i++) {
		sum += Blinking(arr[i], 0, 75);
		printf("Num# %d\n", i);
	}


	//for(int i = 0; i < 50; i++) {
	//	Blink(&arr);
	//	printf("%d: %ld\n", i+25, arrlen(arr));
	//}

	printf("Part 2: %ld\n", sum);

	for(int i = 0; i < hmlen(cache); i++) {
		printf("Key %ld, value %ld\n", i, cache[i].value);
	}
	
	return 0;
}
