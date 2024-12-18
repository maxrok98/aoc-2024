#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH 140
#define HEIGHT 140

char map[HEIGHT][WIDTH] = {0};
char tmap[HEIGHT][WIDTH] = {0};

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
			map[i][j] = line[j];
		}
	}
	free(line);
	fclose(f);
}

typedef struct {
	int perimeter;
	int area;
	int sides;
} Region;

typedef struct {
	int i;
	int j;
} Point;

bool QEmpty(Point** q) {
	return arrlen(*q) == 0;
}

void QPush(Point** q, Point point) {
	arrput(*q, point); 
}

Point QPop(Point** q) {
	Point point = (*q)[0];
	arrdel(*q, 0);
	return point;
}

typedef struct {
	Point key;
	int value;
} Side;

int SidesHorizontaly(Side* s) {
	int sides = 0;
	for(int i = 0; i < hmlen(s); i++) {
		if(s[i].value == 1) {
			s[i].value = -1;
			sides++;

			Point p = s[i].key;
			p.j--;
			while(hmget(s, p)) {
				hmput(s, p, -1);
				p.j--;
			}

			p = s[i].key;
			p.j++;
			while(hmget(s, p)) {
				hmput(s, p, -1);
				p.j++;
			}
		}
	}

	return sides;
}

int SidesVerticaly(Side* s) {
	int sides = 0;
	for(int i = 0; i < hmlen(s); i++) {
		if(s[i].value == 1) {
			s[i].value = -1;
			sides++;

			Point p = s[i].key;
			p.i--;
			while(hmget(s, p)) {
				hmput(s, p, -1);
				p.i--;
			}

			p = s[i].key;
			p.i++;
			while(hmget(s, p)) {
				hmput(s, p, -1);
				p.i++;
			}
		}
	}

	return sides;
}

Region FloodFill(int i, int j) {
	Region region = {0};
	char type = map[i][j];

	Side *N = 0, *S = 0, *E = 0, *W = 0;
	
	Point* q = NULL;
	Point p = { .i = i, .j = j };
	tmap[i][j] = '.';
	QPush(&q, p);
	while(!QEmpty(&q)) {
		p = QPop(&q);
		region.area++;

		bool valid_top = p.i != 0, valid_bottom = p.i != HEIGHT-1;
		bool valid_left = p.j != 0, valid_right = p.j != WIDTH-1; 

		if(!valid_top ||
			(valid_top && (map[p.i-1][p.j] != type))) { hmput(N, p, 1); region.perimeter++; }
		else if(valid_top && tmap[p.i-1][p.j] != '.') { tmap[p.i-1][p.j] = '.'; QPush(&q, (Point) { .i=p.i-1, .j=p.j }); }

		if(!valid_right ||
			(valid_right && (map[p.i][p.j+1] != type))) { hmput(E, p, 1); region.perimeter++; }
		else if(valid_right && tmap[p.i][p.j+1] != '.') { tmap[p.i][p.j+1] = '.'; QPush(&q, (Point) { .i=p.i, .j=p.j+1 }); }

		if(!valid_bottom ||
			(valid_bottom && (map[p.i+1][p.j] != type))) { hmput(S, p, 1); region.perimeter++; }
		else if(valid_bottom && tmap[p.i+1][p.j] != '.') { tmap[p.i+1][p.j] = '.'; QPush(&q, (Point) { .i=p.i+1, .j=p.j }); }

		if(!valid_left ||
			(valid_left && (map[p.i][p.j-1] != type))) { hmput(W, p, 1); region.perimeter++; }
		else if(valid_left && tmap[p.i][p.j-1] != '.') { tmap[p.i][p.j-1] = '.'; QPush(&q, (Point) { .i=p.i, .j=p.j-1 }); }
	}

	region.sides += SidesHorizontaly(N);
	region.sides += SidesHorizontaly(S);
	region.sides += SidesVerticaly(E);
	region.sides += SidesVerticaly(W);

	hmfree(N);
	hmfree(E);
	hmfree(S);
	hmfree(W);
	arrfree(q);
	return region;
}

int main() {
	ParseFile("input.txt");

	int sum = 0;
	int sum2 = 0;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(tmap[i][j] != '.') {
				Region region = FloodFill(i, j);
				sum += region.perimeter * region.area;
				sum2 += region.sides * region.area;
			}	
		}
	}

	printf("Part 1: %d\n", sum);
	printf("Part 2: %d\n", sum2);

	return 0;
}
