#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH 50
#define HEIGHT 50
#define PATH_LENGTH 1000

char map[HEIGHT][WIDTH] = {0};
char* path = NULL;

typedef struct {
	int i;
	int j;
} Point;

Point position;

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
			if(line[j] == '@') {
				position.i = i;
				position.j = j;
				map[i][j] = '.';
			}
			else map[i][j] = line[j];
		}
	}
	getline(&line, &lineLen, f);
	for (int i = 0; getline(&line, &lineLen, f) != -1; i++) {
		for(int j = 0; j < PATH_LENGTH; j++) {
			arrput(path, line[j]);
		}
	}
	free(line);
	fclose(f);
}

Point UpdatePoint(Point p, char direction) {
	Point np = p;
	switch(direction) {
		case '^': np.i--; break;
		case '>': np.j++; break;
		case 'v': np.i++; break;
		case '<': np.j--; break;
		default: printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaa"); exit(1);
	}
	return np;
}
bool TryMove(Point p, char d) {
	if(map[p.i][p.j] == '#') return false;
	if(map[p.i][p.j] == '.') return true;

	if(map[p.i][p.j] == 'O') {
		Point np = UpdatePoint(p, d);
		if(TryMove(np, d)) {
			map[np.i][np.j] = 'O'; 
			map[p.i][p.j] = '.'; 
			return true;
		}
		else return false;
	}
}

void Move(char direction) {
	if(TryMove(UpdatePoint(position, direction), direction)) 
		position = UpdatePoint(position, direction);
}

void DrawMap() {
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(position.i == i && position.j == j) printf("@");
			else printf("%c", map[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

char ToInst(char key) {
	switch(key) {
		case 'w': return '^';
		case 's': return 'v';
		case 'd': return '>';
		case 'a': return '<';
	}
	return 0;
}

int main() {
	ParseFile("input.txt");

	//while(true) {
	//	system("stty raw");
	//	char c = getc(stdin);
	//	system("stty cooked");
	//	if(c) Move(ToInst(c));
	//	DrawMap();
	//}

	DrawMap();

	for(int i = 0; i < arrlen(path); i++) {
		Move(path[i]);
		//DrawMap();
		//usleep(50000);
	}

	int sum = 0;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(map[i][j] == 'O') sum += (100 * i) + j;
		}
	}

	printf("Part 1: %d\n", sum);

	return 0;
}
