#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH  101
#define HEIGHT 103
#define COUNT 500

typedef struct {
	int i;
	int j;
} Point;

typedef struct {
	Point key;
	int value;
} Po;

typedef struct {
	Point position;
	Point velocity;
} Robot;

Robot robots[COUNT];

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
		char* token = strtok(line, "=");
		token = strtok(NULL, ",");
		robots[i].position.j = atoi(token);

		token = strtok(NULL, " ");
		robots[i].position.i = atoi(token);

		token = strtok(NULL, "=");
		token = strtok(NULL, ",");
		robots[i].velocity.j = atoi(token);

		token = strtok(NULL, "\n");
		robots[i].velocity.i = atoi(token);
	}
	free(line);

	fclose(f);
}

bool Q1(Point p) {
	return p.i >= 0 && p.i < (HEIGHT-1)/2 && p.j >= 0 && p.j < (WIDTH-1)/2;
}

bool Q2(Point p) {
	return p.i >= 0 && p.i < (HEIGHT-1)/2 && p.j > (WIDTH-1)/2 && p.j < WIDTH;
}

bool Q3(Point p) {
	return p.i > (HEIGHT-1)/2 && p.i < HEIGHT && p.j >= 0 && p.j < (WIDTH-1)/2;
}

bool Q4(Point p) {
	return p.i > (HEIGHT-1)/2 && p.i < HEIGHT && p.j > (WIDTH-1)/2 && p.j < WIDTH;
}

int mod(int a, int b){
	int r = a % b;
	return r < 0 ? r + b : r;
}

void Simulate(Robot* r, int s) {
	r->position.j += r->velocity.j * s;
	r->position.j = mod(r->position.j, WIDTH);

	r->position.i += r->velocity.i * s;
	r->position.i = mod(r->position.i, HEIGHT);
}

bool SearchLine(Po* points) {
	int minSteps = 10;
	for(int i = 0; i < hmlen(points); i++) {
		int steps = 0;
		Point p = points[i].key;
		while(true) {
			if(steps >= minSteps) return true;
			p.j++; steps++;
			if(!hmget(points, p)) break;
		}
	}
	return false;
}

void PrintState(Po* p) {
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			Point point = { .i = i, .j = j };
			if(hmget(p, point)) printf("#");
			else printf(" ");
		}
		printf("\n");
	}
}

int main() {
	ParseFile("input.txt");

	int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
	for(int i = 0; i < COUNT; i++) {
		Simulate(robots+i, 100);
		if(Q1(robots[i].position)) q1++;
		else if(Q2(robots[i].position)) q2++;
		else if(Q3(robots[i].position)) q3++;
		else if(Q4(robots[i].position)) q4++;
	}

	printf("Part 1: %d\n", q1 * q2 * q3 * q4);

	ParseFile("input.txt");

	int s = 0;
	Po* points = NULL;
	bool found = false;
	while(!found) {
		points = NULL;
		for(int i = 0; i < COUNT; i++) {
			Simulate(robots+i, 1);
			hmput(points, robots[i].position, 1);
		}
		found = SearchLine(points);
		hmfree(points);
		s++;
	}

	printf("Part 2: %d\n", s);
	
	return 0;
}
