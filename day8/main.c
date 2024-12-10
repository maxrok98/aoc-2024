#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH 50
#define HEIGHT 50

typedef struct {
	int i;
	int j;
} Point;

typedef struct {
	char key;
	Point* value;
} LettersDict;

typedef struct {
	Point key;
	int value;
} PointsDict;

LettersDict* ParseFile(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	LettersDict* dict = NULL;
	fseek(f, 0, SEEK_SET);
	char* line = NULL;
	size_t lineLen;
	for (int i = 0; getline(&line, &lineLen, f) != -1; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(line[j] != '.') {
				Point letterPosition = { .i = i, .j = j, };
				Point* p = hmget(dict, line[j]);
				if(p) {
					arrput(p, letterPosition);
				}
				else {
					arrput(p, letterPosition);
					hmput(dict, line[j], p);
				}
			}
		}
	}
	free(line);

	fclose(f);
	return dict;
}

bool ValidPoint(Point p) {
	return (p.i >= 0 && p.i < HEIGHT) && (p.j >= 0 && p.j < WIDTH);
}

void SaveAntinodes1(Point p1, Point p2, PointsDict** pointsDict) {
	int di = p1.i - p2.i;
	int dj = p1.j - p2.j;

	Point antinode1 = {
		.i = p1.i + di,
		.j = p1.j + dj
	};
	if(ValidPoint(antinode1)) hmput(*pointsDict, antinode1, 1);

	Point antinode2 = {
		.i = p2.i - di,
		.j = p2.j - dj
	};
	if(ValidPoint(antinode2)) hmput(*pointsDict, antinode2, 1);
}

void SaveAntinodes2(Point p1, Point p2, PointsDict** pointsDict) {
	hmput(*pointsDict, p1, 1);
	hmput(*pointsDict, p2, 1);
	int di = p1.i - p2.i;
	int dj = p1.j - p2.j;

	Point antinode1 = {
		.i = p1.i + di,
		.j = p1.j + dj
	};
	while(ValidPoint(antinode1)) {
		hmput(*pointsDict, antinode1, 1);
		antinode1.i+=di;
		antinode1.j+=dj;
	}

	Point antinode2 = {
		.i = p2.i - di,
		.j = p2.j - dj
	};
	while(ValidPoint(antinode2)) {
		hmput(*pointsDict, antinode2, 1);
		antinode2.i-=di;
		antinode2.j-=dj;
	}
}

PointsDict* FindAntinodes1(LettersDict* lettersDict) {
	PointsDict* pointsDict = NULL;
	for(int i = 0; i < hmlen(lettersDict); i++) {
		Point* points = lettersDict[i].value;
		for(int j = 0; j < arrlen(points)-1; j++) {
			for(int k = j+1; k < arrlen(points); k++) {
				SaveAntinodes1(points[j], points[k], &pointsDict);
			}
		}
	}
	return pointsDict;
}

PointsDict* FindAntinodes2(LettersDict* lettersDict) {
	PointsDict* pointsDict = NULL;
	for(int i = 0; i < hmlen(lettersDict); i++) {
		Point* points = lettersDict[i].value;
		for(int j = 0; j < arrlen(points)-1; j++) {
			for(int k = j+1; k < arrlen(points); k++) {
				SaveAntinodes2(points[j], points[k], &pointsDict);
			}
		}
	}
	return pointsDict;
}

int main() {
	LettersDict* lettersDict = ParseFile("input.txt");
	PointsDict* pointsDict = FindAntinodes1(lettersDict);
	printf("Part 1: %ld\n", hmlen(pointsDict));

	pointsDict = FindAntinodes2(lettersDict);
	printf("Part 2: %ld\n", hmlen(pointsDict));

	return 0;
}

