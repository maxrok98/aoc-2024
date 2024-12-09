#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define HEIGHT 130
#define WIDTH 130
#define ARRAY_LENGTH 6

typedef struct {
	int i;
	int j;
} Point;

typedef enum {
	UP,
	RIGHT,
	DOWN,
	LEFT
} Direction;

typedef struct state {
	char map[HEIGHT][WIDTH];
	Point point;
	Direction direction;

	int stepedPositions;
	int possibleLoops;

	bool endGame;
} State;

void ParseFile(char* fileName, State* state) {
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
			if(line[j] == '#') state->map[i][j] = '#';
			if(line[j] == '^') { 
				state->point.i = i; state->point.j = j;
				state->direction = UP;
			}
		}
	}
	free(line);
	fclose(f);
}

void DrawMap(State* state) {
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(state->point.i == i && state->point.j == j) {
				printf(ANSI_COLOR_GREEN);
				switch(state->direction) {
					case UP:
						printf("/\\");
						break;
					case RIGHT:
						printf(" >");
						break;
					case DOWN:
						printf("\\/");
						break;
					case LEFT:
						printf("< ");
						break;
				}				
				printf(ANSI_COLOR_RESET);
			}
			else if(state->map[i][j] == '#') printf("\u2588\u2588");
			else if(state->map[i][j] == 'X') printf(ANSI_COLOR_YELLOW "\u2588\u2588" ANSI_COLOR_RESET);
			else printf("  ");
		}
		printf("\n");
	}
}

void UpdateDirection(State* state) {
	state->direction++;
	state->direction %= 4;
}

void MarkPlace(State* state) {
	if(state->map[state->point.i][state->point.j] != 'X') {
		state->map[state->point.i][state->point.j] = 'X';
		state->stepedPositions++;
	}
}

void AdvanceState(State* state, bool loopCheck) {
	int valid_top = (state->point.i != 0), valid_bottom = (state->point.i != HEIGHT-1);
	int valid_left = (state->point.j != 0), valid_right = (state->point.j != WIDTH-1); 

	if((!valid_top && state->direction == UP) ||
		 (!valid_right && state->direction == RIGHT) ||
		 (!valid_bottom && state->direction == DOWN) ||
		 (!valid_left && state->direction == LEFT)
	) {
		if(!loopCheck) MarkPlace(state);
		state->endGame = true;
		return;
	}

	if(state->direction == UP && state->map[state->point.i-1][state->point.j] != '#') {
		if(!loopCheck) MarkPlace(state);
		state->point.i--;
	}
	else if(state->direction == RIGHT && state->map[state->point.i][state->point.j+1] != '#') {
		if(!loopCheck) MarkPlace(state);
		state->point.j++;
	}
	else if(state->direction == DOWN && state->map[state->point.i+1][state->point.j] != '#') {
		if(!loopCheck) MarkPlace(state);
		state->point.i++;
	}
	else if(state->direction == LEFT && state->map[state->point.i][state->point.j-1] != '#') {
		if(!loopCheck) MarkPlace(state);
		state->point.j--;
	}
	else { // hit the wall
		UpdateDirection(state);
		AdvanceState(state, loopCheck);
	}
}

bool CheckPossibleLoop(State* state, int* wi, int* wj) {
	bool loop = false;

	typedef struct {
		Point point;
		Direction direction;
	} Position;

	struct { Position key; int value; }* hash = NULL;
	while(true) {
		if(state->endGame) { loop = false; break; }

		Position p = {
			.point = {
				.i = state->point.i,
				.j = state->point.j,
			},
			.direction = state->direction
		};
		if(hmget(hash, p)) { loop = true; break; }
		hmput(hash, p, 1);

		AdvanceState(state, true);
	}

	hmfree(hash);
	return loop;
}

int main() {
	State state = {0};

	ParseFile("input.txt", &state);
	Point startingPoint = state.point;
	Direction startingDirection = state.direction;

	while(!state.endGame) {
		AdvanceState(&state, false);

		//DrawMap(&state);
		//usleep(12000);
	}

	//state.point = startingPoint;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(i == startingPoint.i && j == startingPoint.j) continue;
			if(state.map[i][j] == 'X') {
				state.map[i][j] = '#';
				int wi, wj;
				state.point = startingPoint;
				state.direction = startingDirection;
				state.endGame = false;
				if(CheckPossibleLoop(&state, &wi, &wj)) {
					state.possibleLoops++;
				}
				state.map[i][j] = 'X';
			}
		}
	}

	printf("Part 1: %d\n", state.stepedPositions);
	printf("Part 2: %d\n", state.possibleLoops);
	
	return 0;
}

