#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define ARRAY_LENGTH 6
#define HEIGHT 140
#define WIDTH 140

char* XMAS = "XMAS";
char* MAS = "MAS";

typedef enum {
	TOP,
	TOP_LEFT,
	TOP_RIGHT,
	LEFT,
	RIGHT,
	BOTTOM,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	NO
} Direction;

void ParseFile(char* fileName, char* letters) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	char* line = NULL;
	size_t lineLen;
	for (int i = 0; i < HEIGHT; i++) {
 		getline(&line, &lineLen, f);
		memcpy(letters+(i*WIDTH), line, WIDTH);
	}
	free(line);
	fclose(f);
}

char letter(char* letters, int i, int j) {
	return *(letters+(i*WIDTH)+j);
}

int SearchXmas(char* letters, int i, int j, int letterNum, Direction direction) {
	int valid_top = (i != 0), valid_bottom = (i != HEIGHT-1);
	int valid_left = (j != 0), valid_right = (j != WIDTH-1); 

	int sum = 0;
	if(XMAS[letterNum] == 'S') 				return 1;

	if(valid_top && valid_left && 	  (direction == NO || direction == TOP_LEFT		 ) &&	letter(letters, i-1, j-1) == XMAS[letterNum+1])	sum += SearchXmas(letters, i-1, j-1, letterNum+1, direction == NO ? TOP_LEFT 		 : direction);
	if(valid_top && 								  (direction == NO || direction == TOP		 		 ) && letter(letters, i-1, j  ) == XMAS[letterNum+1])	sum += SearchXmas(letters, i-1, j,   letterNum+1, direction == NO ? TOP			 		 : direction);
	if(valid_top && valid_right && 	  (direction == NO || direction == TOP_RIGHT	 ) && letter(letters, i-1, j+1) == XMAS[letterNum+1])	sum += SearchXmas(letters, i-1, j+1, letterNum+1, direction == NO ? TOP_RIGHT 	 : direction);
	if(valid_left && 								  (direction == NO || direction == LEFT				 ) && letter(letters, i,   j-1) == XMAS[letterNum+1])	sum += SearchXmas(letters, i,   j-1, letterNum+1, direction == NO ? LEFT 				 : direction);
	if(valid_right && 							  (direction == NO || direction == RIGHT			 ) && letter(letters, i,   j+1) == XMAS[letterNum+1])	sum += SearchXmas(letters, i,   j+1, letterNum+1, direction == NO ? RIGHT 			 : direction);
	if(valid_bottom && valid_left &&  (direction == NO || direction == BOTTOM_LEFT ) &&	letter(letters, i+1, j-1) == XMAS[letterNum+1])	sum += SearchXmas(letters, i+1, j-1, letterNum+1, direction == NO ? BOTTOM_LEFT  : direction);
	if(valid_bottom && 							  (direction == NO || direction == BOTTOM			 ) && letter(letters, i+1, j  ) == XMAS[letterNum+1])	sum += SearchXmas(letters, i+1, j,   letterNum+1, direction == NO ? BOTTOM 			 : direction);
	if(valid_bottom && valid_right && (direction == NO || direction == BOTTOM_RIGHT) && letter(letters, i+1, j+1) == XMAS[letterNum+1]) sum += SearchXmas(letters, i+1, j+1, letterNum+1, direction == NO ? BOTTOM_RIGHT : direction);

	return sum;
}

typedef struct {
	int i;
	int j;
} Point;

typedef struct {
	Point* points;
	int count;
	int allocated;
} PointArray;
PointArray pointArray;

void InitPointArray(PointArray* pointArray) {
	pointArray->count = 0;
	pointArray->allocated = ARRAY_LENGTH;
	pointArray->points = (Point*)calloc(ARRAY_LENGTH, sizeof(Point));
}

void AddPoint(PointArray* pointArray, Point point) {
	if(pointArray->count == pointArray->allocated) {
		pointArray->points = (Point*)realloc(pointArray->points, sizeof(Point)*(pointArray->allocated*=2));
	}
	pointArray->points[pointArray->count++] = point;
}

bool ContainsPoint(PointArray* pointArray, Point point) {
	for(int i = 0; i < pointArray->count; i++) {
		if(point.i == pointArray->points[i].i && point.j == pointArray->points[i].j) return true;
	}
	return false;
}

int VerifyMas(char* letters, int i, int j, Direction direction) {
	assert(letter(letters, i, j) == 'A');
	int valid_top = (i != 0), valid_bottom = (i != HEIGHT-1);
	int valid_left = (j != 0), valid_right = (j != WIDTH-1); 
	if(!(valid_top && valid_bottom && valid_left && valid_right)) return 0;

	if(direction == TOP_LEFT &&	letter(letters, i-1, j-1) == 'S') {
		if(letter(letters, i+1, j-1) == 'M' && letter(letters, i-1, j+1) == 'S') {
			Point point = { .i = i+1, .j = j - 1};
			//AddPoint(&pointArray, point);
			return 1;
		}
		if(letter(letters, i+1, j-1) == 'S' && letter(letters, i-1, j+1) == 'M') {
			Point point = { .i = i-1, .j = j+1};
			//AddPoint(&pointArray, point);
			return 1;
		}
	}
	if(direction == TOP_RIGHT &&	letter(letters, i-1, j+1) == 'S') {
		if(letter(letters, i-1, j-1) == 'M' && letter(letters, i+1, j+1) == 'S') {
			Point point = { .i = i-1, .j = j-1};
			//AddPoint(&pointArray, point);
			return 1;
		}
		if(letter(letters, i+1, j+1) == 'M' && letter(letters, i-1, j-1) == 'S') {
			Point point = { .i = i+1, .j = j+1};
			//AddPoint(&pointArray, point);
			return 1;
		}
	}
	if(direction == BOTTOM_LEFT &&	letter(letters, i+1, j-1) == 'S') {
		if(letter(letters, i-1, j-1) == 'M' && letter(letters, i+1, j+1) == 'S') {
			Point point = { .i = i-1, .j = j-1};
			//AddPoint(&pointArray, point);
			return 1;
		}
		if(letter(letters, i+1, j+1) == 'M' && letter(letters, i-1, j-1) == 'S') {
			Point point = { .i = i+1, .j = j+1};
			//AddPoint(&pointArray, point);
			return 1;
		}
	}
	if(direction == BOTTOM_RIGHT &&	letter(letters, i+1, j+1) == 'S') {
		if(letter(letters, i+1, j-1) == 'M' && letter(letters, i-1, j+1) == 'S') {
			Point point = { .i = i+1, .j = j-1};
			//AddPoint(&pointArray, point);
			return 1;
		}
		if(letter(letters, i+1, j-1) == 'S' && letter(letters, i-1, j+1) == 'M') {
			Point point = { .i = i-1, .j = j+1};
			//AddPoint(&pointArray, point);
			return 1;
		}
	}

	return 0;
}


int SearchMas(char* letters, int i, int j, int letterNum) {
	int valid_top = (i != 0), valid_bottom = (i != HEIGHT-1);
	int valid_left = (j != 0), valid_right = (j != WIDTH-1); 

	int sum = 0;

	if(valid_top && valid_left && 	 	letter(letters, i-1, j-1) == MAS[letterNum+1])	sum += VerifyMas(letters, i-1, j-1, TOP_LEFT 		);
	if(valid_top && valid_right && 	  letter(letters, i-1, j+1) == MAS[letterNum+1])	sum += VerifyMas(letters, i-1, j+1, TOP_RIGHT 	);
	if(valid_bottom && valid_left && 	letter(letters, i+1, j-1) == MAS[letterNum+1])	sum += VerifyMas(letters, i+1, j-1, BOTTOM_LEFT );
	if(valid_bottom && valid_right && letter(letters, i+1, j+1) == MAS[letterNum+1]) sum += VerifyMas(letters, i+1, j+1, BOTTOM_RIGHT);

	return sum;
}

int main() {
	char letters[HEIGHT][WIDTH];

	ParseFile("input.txt", letters);

	int sum = 0;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(letter(letters, i, j) == XMAS[0])
				sum += SearchXmas(letters, i, j, 0, NO);
		}
	}
	printf("Part 1: %d\n", sum);

	InitPointArray(&pointArray);
	int sum2 = 0;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			Point point = { .i = i, .j = j };
			if(letter(letters, i, j) == MAS[0] && !ContainsPoint(&pointArray, point))
				sum2 += SearchMas(letters, i, j, 0);
		}
	}
	printf("Part 2: %d\n", sum2/2);
	
	return 0;
}
