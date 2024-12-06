#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

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

typedef struct {
	Point* points;
	int count;
	int allocated;
} PointArray;

void InitArray(PointArray* array) {
	array->count = 0;
	array->allocated = ARRAY_LENGTH;
	array->points = (Point*)calloc(ARRAY_LENGTH, sizeof(Point));
}

void AddPoint(PointArray* array, Point point) {
	if(array->count == array->allocated) {
		array->points = (Point*)realloc(array->points, sizeof(Point)*(array->allocated*=2));
	}
	array->points[array->count++] = point;
}

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
	PointArray turnPositions;

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
			if(state->map[i][j] == '#') printf("\u2588\u2588");
			else if(state->map[i][j] == 'X') printf(ANSI_COLOR_YELLOW "\u2588\u2588" ANSI_COLOR_RESET);
			else if(state->point.i == i && state->point.j == j) {
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

int Max(int a, int b) {
	return a >= b ? a : b;
}

int Min(int a, int b) {
	return a < b ? a : b;
}

bool FreeWay(State* state, Point p1, Point p2) {
	assert((p1.i == p2.i) || (p1.j == p2.j));
	if(p1.i == p2.i) {
		int i = p1.i;
		for(int j = Min(p1.j, p2.j); j < Max(p1.j, p2.j); j++) {
			if(state->map[i][j] == '#') return false;
		}
	}
	else if(p1.j == p2.j) {
		int j = p1.j;
		for(int i = Min(p1.i, p2.i); i < Max(p1.i, p2.i); i++) {
			if(state->map[i][j] == '#') return false;
		}
	}
	return true;
}

void CheckPossibleLoop(State* state) {
	if(state->turnPositions.count < 3) return;
	PointArray turnHistory = state->turnPositions;
	if(((state->direction == UP || state->direction == DOWN) && state->point.i == turnHistory.points[turnHistory.count-3].i) ||
		 ((state->direction == RIGHT || state->direction == LEFT) && state->point.j == turnHistory.points[turnHistory.count-3].j)) {
		if(FreeWay(state, state->point, turnHistory.points[turnHistory.count-3])) state->possibleLoops++;
	}
}

void AdvanceState(State* state) {
	int valid_top = (state->point.i != 0), valid_bottom = (state->point.i != HEIGHT-1);
	int valid_left = (state->point.j != 0), valid_right = (state->point.j != WIDTH-1); 

	if((!valid_top && state->direction == UP) ||
		 (!valid_right && state->direction == RIGHT) ||
		 (!valid_bottom && state->direction == DOWN) ||
		 (!valid_left && state->direction == LEFT)
	) {
		MarkPlace(state);
		state->endGame = true;
		return;
	}

	CheckPossibleLoop(state);

	if(state->direction == UP && state->map[state->point.i-1][state->point.j] != '#') {
		MarkPlace(state);
		state->point.i--;
	}
	else if(state->direction == RIGHT && state->map[state->point.i][state->point.j+1] != '#') {
		MarkPlace(state);
		state->point.j++;
	}
	else if(state->direction == DOWN && state->map[state->point.i+1][state->point.j] != '#') {
		MarkPlace(state);
		state->point.i++;
	}
	else if(state->direction == LEFT && state->map[state->point.i][state->point.j-1] != '#') {
		MarkPlace(state);
		state->point.j--;
	}
	else { // hit the wall
		UpdateDirection(state);
		AddPoint(&state->turnPositions, state->point);
	}
}

int main() {
	State state = {0};
	InitArray(&state.turnPositions);

	ParseFile("input.txt", &state);

	while(!state.endGame) {
		AdvanceState(&state);

		//DrawMap(&state);
		//usleep(25000);
	}

	printf("Part 1: %d\n", state.stepedPositions);
	printf("Part 2: %d\n", state.possibleLoops);
	
	return 0;
}

