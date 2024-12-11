#include <stdio.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define HEIGHT 40
#define WIDTH 40

int map[HEIGHT][WIDTH] = {0};

int ctoi(char c) {
	return (int)(c - 48);
}

void ParseFile(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	char* line = NULL;
	size_t lineLen;
	for (int i = 0; i < HEIGHT; i++) {
 		getline(&line, &lineLen, f);
		for(int j = 0; j < WIDTH; j++) {
			map[i][j] = ctoi(line[j]);
		}
	}
	free(line);
	fclose(f);
}

typedef struct {
	int i;
	int j;
} Point;

typedef struct {
	Point key;
	int value;
} Routs;

void FindRouts(int i, int j, Routs** routs) {
	if(map[i][j] == 9) {
		Point p = { .i = i, .j = j };
		int value = hmget((*routs), p);
		if(value) hmput(*routs, p, value+1);
		else hmput(*routs, p, 1);
		return;
	}
	int valid_top = i != 0, valid_bottom = i != HEIGHT-1;
	int valid_left = j != 0, valid_right = j != WIDTH-1; 
	
	if(valid_top && map[i-1][j] - map[i][j] == 1) FindRouts(i-1, j, routs);
	if(valid_right && map[i][j+1] - map[i][j] == 1) FindRouts(i, j+1, routs);
	if(valid_bottom && map[i+1][j] - map[i][j] == 1) FindRouts(i+1, j, routs);
	if(valid_left && map[i][j-1] - map[i][j] == 1) FindRouts(i, j-1, routs);
}

int main() {
	ParseFile("input.txt");

	int sum1 = 0;
	int sum2 = 0;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(map[i][j] == 0) {
				Routs* routs = NULL;
				FindRouts(i, j, &routs);
				sum1 += hmlen(routs);
				for(int k = 0; k < hmlen(routs); k++) {
					sum2 += routs[k].value;
				}
			}
		}
	}

	printf("Part 1: %d\n", sum1);
	printf("Part 2: %d\n", sum2);
	
	return 0;
}
