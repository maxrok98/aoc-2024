#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define ARRAY_LENGTH 6
#define HEIGHT 140
#define WIDTH 140

char letters[HEIGHT][WIDTH];
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

int SearchXmas(int i, int j, int letterNum, Direction direction) {
	int valid_top = (i != 0), valid_bottom = (i != HEIGHT-1);
	int valid_left = (j != 0), valid_right = (j != WIDTH-1); 

	int sum = 0;
	if(XMAS[letterNum] == 'S') 				return 1;

	if(valid_top && valid_left && 	  (direction == NO || direction == TOP_LEFT		 ) &&	letters[i-1][j-1] == XMAS[letterNum+1])	sum += SearchXmas(i-1, j-1, letterNum+1, direction == NO ? TOP_LEFT 		: direction);
	if(valid_top && 								  (direction == NO || direction == TOP		 		 ) && letters[i-1][j  ] == XMAS[letterNum+1])	sum += SearchXmas(i-1, j,   letterNum+1, direction == NO ? TOP			 		: direction);
	if(valid_top && valid_right && 	  (direction == NO || direction == TOP_RIGHT	 ) && letters[i-1][j+1] == XMAS[letterNum+1])	sum += SearchXmas(i-1, j+1, letterNum+1, direction == NO ? TOP_RIGHT 	  : direction);
	if(valid_left && 								  (direction == NO || direction == LEFT				 ) && letters[i  ][j-1] == XMAS[letterNum+1])	sum += SearchXmas(i,   j-1, letterNum+1, direction == NO ? LEFT 				: direction);
	if(valid_right && 							  (direction == NO || direction == RIGHT			 ) && letters[i  ][j+1] == XMAS[letterNum+1])	sum += SearchXmas(i,   j+1, letterNum+1, direction == NO ? RIGHT 			  : direction);
	if(valid_bottom && valid_left &&  (direction == NO || direction == BOTTOM_LEFT ) &&	letters[i+1][j-1] == XMAS[letterNum+1])	sum += SearchXmas(i+1, j-1, letterNum+1, direction == NO ? BOTTOM_LEFT  : direction);
	if(valid_bottom && 							  (direction == NO || direction == BOTTOM			 ) && letters[i+1][j  ] == XMAS[letterNum+1])	sum += SearchXmas(i+1, j,   letterNum+1, direction == NO ? BOTTOM 			: direction);
	if(valid_bottom && valid_right && (direction == NO || direction == BOTTOM_RIGHT) && letters[i+1][j+1] == XMAS[letterNum+1]) sum += SearchXmas(i+1, j+1, letterNum+1, direction == NO ? BOTTOM_RIGHT : direction);

	return sum;
}

int VerifyMas(int i, int j, Direction direction) {
	assert(letters[i][j] == 'A');
	int valid_top = (i != 0), valid_bottom = (i != HEIGHT-1);
	int valid_left = (j != 0), valid_right = (j != WIDTH-1); 
	if(!(valid_top && valid_bottom && valid_left && valid_right)) return 0;

	if(direction == TOP_LEFT &&	letters[i-1][j-1] == 'S') {
		if((letters[i+1][j-1] == 'M' && letters[i-1][j+1] == 'S') ||
		   (letters[i+1][j-1] == 'S' && letters[i-1][j+1] == 'M')) {
			return 1;
		}
	}
	if(direction == TOP_RIGHT && letters[i-1][j+1] == 'S') {
		if((letters[i-1][j-1] == 'M' && letters[i+1][j+1] == 'S') ||
		   (letters[i+1][j+1] == 'M' && letters[i-1][j-1] == 'S')) {
			return 1;
		}
	}
	if(direction == BOTTOM_LEFT && letters[i+1][j-1] == 'S') {
		if((letters[i-1][j-1] == 'M' && letters[i+1][j+1] == 'S') ||
			 (letters[i+1][j+1] == 'M' && letters[i-1][j-1] == 'S')) {
			return 1;
		}
	}
	if(direction == BOTTOM_RIGHT &&	letters[i+1][j+1] == 'S') {
		if((letters[i+1][j-1] == 'M' && letters[i-1][j+1] == 'S') ||
			 (letters[i+1][j-1] == 'S' && letters[i-1][j+1] == 'M')) {
			return 1;
		}
	}

	return 0;
}

int SearchMas(int i, int j, int letterNum) {
	int valid_top = (i != 0), valid_bottom = (i != HEIGHT-1);
	int valid_left = (j != 0), valid_right = (j != WIDTH-1); 

	int sum = 0;

	if(valid_top && valid_left && 	 	letters[i-1][j-1] == MAS[letterNum+1])	sum += VerifyMas(i-1, j-1, TOP_LEFT 	 );
	if(valid_top && valid_right && 	  letters[i-1][j+1] == MAS[letterNum+1])	sum += VerifyMas(i-1, j+1, TOP_RIGHT 	 );
	if(valid_bottom && valid_left && 	letters[i+1][j-1] == MAS[letterNum+1])	sum += VerifyMas(i+1, j-1, BOTTOM_LEFT );
	if(valid_bottom && valid_right && letters[i+1][j+1] == MAS[letterNum+1]) 	sum += VerifyMas(i+1, j+1, BOTTOM_RIGHT);

	return sum;
}

int main() {
	ParseFile("input.txt", letters);

	int sum = 0;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(letters[i][j] == XMAS[0])
				sum += SearchXmas(i, j, 0, NO);
		}
	}
	printf("Part 1: %d\n", sum);

	int sum2 = 0;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(letters[i][j] == MAS[0])
				sum2 += SearchMas(i, j, 0);
		}
	}
	printf("Part 2: %d\n", sum2/2);
	
	return 0;
}
