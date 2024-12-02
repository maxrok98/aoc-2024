#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ARRAY_LENGTH 6

typedef struct {
	int* levels;
	int count;
	int allocated;
} Report;

void InitReport(Report* report) {
	report->count = 0;
	report->allocated = ARRAY_LENGTH;
	report->levels = (int*)calloc(ARRAY_LENGTH, sizeof(int));
}

void AddLevel(Report* report, int level) {
	if(report->count == report->allocated) {
		report->levels = (int*)realloc(report->levels, sizeof(int)*report->allocated*2);
	}
	report->levels[report->count++] = level;
}

int GetLines(FILE* file) {
	char ch;
	int linesCount = 0;
	while((ch = fgetc(file)) != EOF) {
		if(ch == '\n')
			linesCount++;
	}

	return linesCount;
}

int ParseFile(char* fileName, Report** reports) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}
	int linesCount = GetLines(f);
	*reports = (Report*)malloc(sizeof(Report)*linesCount);

	fseek(f, 0, SEEK_SET);
	char* line = NULL;
	size_t lineLen;
	for (int i = 0; getline(&line, &lineLen, f) != -1; i++) {
		InitReport(*reports+i);
		char* token = strtok(line, " ");
		while(token != NULL) {
			AddLevel(*reports+i, atoi(token));
			token = strtok(NULL, " ");
		}
	}
	free(line);

	fclose(f);
	return linesCount;
}

bool SafeReport(Report* report) {
	bool increasing = report->levels[0] < report->levels[1];
	int prevLevel = report->levels[0];
	for(int i = 1; i < report->count; i++) {
		int diff = abs(prevLevel - report->levels[i]);
		if(!((prevLevel < report->levels[i]) == increasing) || !(diff >= 1 && diff <= 3)) {
			return false;
		}
		prevLevel = report->levels[i];
	}
	return true;
}

bool isIncreasing(int* levels, int i, int j) {
	return levels[i] < levels[j];
}

bool SafeReportWithSkip(Report* report, int skipLevel) {
	bool increasing = isIncreasing(report->levels, 0, 1);
	int prevLevel = report->levels[0];
	int i = 1;
	if(skipLevel == 0) {
		increasing = isIncreasing(report->levels, 1, 2);
		prevLevel = report->levels[1];
		i = 2;
	}
	else if(skipLevel == 1) {
		increasing = isIncreasing(report->levels, 0, 2);
		i = 2;
	}
		
	for(; i < report->count; i++) {
		if(i == skipLevel) continue;
		int diff = abs(prevLevel - report->levels[i]);
		if(!((prevLevel < report->levels[i]) == increasing) || !(diff >= 1 && diff <= 3)) {
			return false;
		}
		prevLevel = report->levels[i];
	}
	return true;
}

bool ActualSafeReport(Report* report) {
	for(int i = 0; i < report->count; i++){
		if(SafeReportWithSkip(report, i)) return true;
	}
	return false;
}

int main() {
	Report* reports;
	int count = ParseFile("input.txt", &reports);

	int sum1 = 0;
	for(int i = 0; i < count; i++) {
		if(SafeReport(&reports[i])) {
			sum1++;
		}
	}
	printf("Part 1: %d\n", sum1);

	int sum2 = 0;
	for(int i = 0; i < count; i++) {
		if(ActualSafeReport(&reports[i])) {
			sum2++;
		}
	}
	printf("Part 2: %d\n", sum2);

	return 0;
}
