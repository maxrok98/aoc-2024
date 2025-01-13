#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH  141
#define HEIGHT  141
//#define VISITED 0
#define SPACE -1
#define WALL -2

int map[HEIGHT][WIDTH] = {0};

typedef struct {
	int i;
	int j;
} Point;

Point start;
Point end;

typedef enum {
	UP,
	RIGHT,
	DOWN,
	LEFT
} Direction;

Point GetNextPoint(Point p, Direction d) {
	Point np = p;
	switch(d) {
		case UP: np.i--; break;
		case RIGHT: np.j++; break;
		case DOWN: np.i++; break;
		case LEFT: np.j--; break;
	}
	return np;
}

Point GetNextShort(Point p, Direction d) {
	Point np = p;
	switch(d) {
		case UP: np.i-=2; break;
		case RIGHT: np.j+=2; break;
		case DOWN: np.i+=2; break;
		case LEFT: np.j-=2; break;
	}
	return np;
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
			if(line[j] == 'S') {
				start.i = i;
				start.j = j;
				map[i][j] = SPACE;
			}
			else if(line[j] == 'E') {
				end.i = i;
				end.j = j;
				map[i][j] = SPACE;
			}
			else if(line[j] == '.') {
				map[i][j] = SPACE;
			}
			else if(line[j] == '#') {
				map[i][j] = WALL;
			}
		}
	}
	free(line);
	fclose(f);
}

void MarkTrack() {
	Point p = start;
	int picoseconds = 0;
	map[p.i][p.j] = picoseconds;

	while(memcmp(&p, &end, sizeof(Point)) != 0) {
		Direction d;
		Point np;
		for(d = 0; d < 4; d++) {
			np = GetNextPoint(p, d);
			if(map[np.i][np.j] == SPACE) break;
		}

		map[np.i][np.j] = ++picoseconds;
		p = np;
	}
}

bool ValidPoint(Point p) {
	return p.i > 0 && p.i < HEIGHT - 1 && p.j > 0 && p.j < WIDTH - 1;
}

int FindShortPaths(int minShortage) {
	int paths = 0;
	Point p = start;

	while(memcmp(&p, &end, sizeof(Point)) != 0) {
		//Direction d;
		Point np;
		for(Direction d = 0; d < 4; d++) {
			np = GetNextPoint(p, d);
			if((map[np.i][np.j] >= 0) && (map[np.i][np.j] - map[p.i][p.j]) == 1) break;
		}

		for(Direction d = 0; d < 4; d++) {
			Point pp = GetNextShort(p, d);
			if(ValidPoint(pp) &&
				(map[pp.i][pp.j] >= 0) &&
				(map[pp.i][pp.j] - map[p.i][p.j] - 2) >= minShortage ) paths++;
		}

		p = np;
	}

	return paths;
}

int FindShortPaths2(int maxSkip, int minShortage) {
	int paths = 0;
	Point p = start;

	while(memcmp(&p, &end, sizeof(Point)) != 0) {
		//Direction d;
		Point np;
		for(Direction d = 0; d < 4; d++) {
			np = GetNextPoint(p, d);
			if((map[np.i][np.j] >= 0) && (map[np.i][np.j] - map[p.i][p.j]) == 1) break;
		}

		for(int i = p.i-maxSkip, j = p.j; j >= p.j-maxSkip;  j--) {
			for(int ii = i, jj = j, count = 0; count <= maxSkip; ii++, jj++, count++) {
				Point pp = { .i = ii, .j = jj };
				int id = abs(ii - p.i);
				int jd = abs(jj - p.j);
				int distance = id + jd;
				if(ValidPoint(pp) &&
					(map[pp.i][pp.j] >= 0) &&
					(map[pp.i][pp.j] - map[p.i][p.j] - distance) >= minShortage ) paths++;
			}
			if(j == p.j-maxSkip) break;
			i++;
			for(int ii = i, jj = j, count = 0; count < maxSkip; ii++, jj++, count++) {
				Point pp = { .i = ii, .j = jj };
				int id = abs(ii - p.i);
				int jd = abs(jj - p.j);
				int distance = id + jd;
				if(ValidPoint(pp) &&
					(map[pp.i][pp.j] >= 0) &&
					(map[pp.i][pp.j] - map[p.i][p.j] - distance) >= minShortage ) paths++;
			}
		}

		p = np;
	}

	return paths;
}

int main() {
	ParseFile("input.txt");
	MarkTrack();
	int shortPath = FindShortPaths(12);
	printf("%d\n", shortPath);

	int shortPath2 = FindShortPaths2(20, 100);
	printf("%d\n", shortPath2);

	return 0;
}
