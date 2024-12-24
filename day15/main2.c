#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH 50
#define HEIGHT 50
#define PATH_LENGTH 1000

char map[HEIGHT][WIDTH*2] = {0};
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
			switch(line[j]) {
				case '@': 
					position.i = i;
					position.j = j*2;
					map[i][j*2] = '.';
					map[i][j*2+1] = '.';
					break;
				case '#':
					map[i][j*2] = '#';
					map[i][j*2+1] = '#';
					break;
				case 'O':
					map[i][j*2] = '[';
					map[i][j*2+1] = ']';
					break;
				case '.':
					map[i][j*2] = '.';
					map[i][j*2+1] = '.';
					break;
			}
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
bool TryMoveH(Point p, char d) {
	if(map[p.i][p.j] == '#') return false;
	if(map[p.i][p.j] == '.') return true;

	if(map[p.i][p.j] == '[' || map[p.i][p.j] == ']') {
		Point np = UpdatePoint(p, d);
		if(TryMoveH(np, d)) {
			map[np.i][np.j] = map[p.i][p.j]; 
			map[p.i][p.j] = '.'; 
			return true;
		}
		else return false;
	}
}

bool Movable(Point p, char d) {
	if(map[p.i][p.j] == '#') return false;
	if(map[p.i][p.j] == '.') return true;

	Point leftParen, rightParen;	
	if(map[p.i][p.j] == '[') {
		leftParen = rightParen = p;
		rightParen.j++;
	}
	if(map[p.i][p.j] == ']') {
		leftParen = rightParen = p;
		leftParen.j--;
	}
	return Movable(UpdatePoint(leftParen, d), d) && Movable(UpdatePoint(rightParen, d), d);
}

void MoveH(Point p, char d) {
	Point leftParen, rightParen;	
	if(map[p.i][p.j] == '[') {
		leftParen = rightParen = p;
		rightParen.j++;
	}
	if(map[p.i][p.j] == ']') {
		leftParen = rightParen = p;
		leftParen.j--;
	}

	Point np = UpdatePoint(leftParen, d);
	if(map[np.i][np.j] != '.') MoveH(np, d);
	map[np.i][np.j] = map[leftParen.i][leftParen.j];
	map[leftParen.i][leftParen.j] = '.';

	np = UpdatePoint(rightParen, d);
	if(map[np.i][np.j] != '.') MoveH(np, d);
	map[np.i][np.j] = map[rightParen.i][rightParen.j];
	map[rightParen.i][rightParen.j] = '.';
}

bool TryMoveV(Point p, char d) {
	if(map[p.i][p.j] == '#') return false;
	if(map[p.i][p.j] == '.') return true;

	if(map[p.i][p.j] == '[' || map[p.i][p.j] == ']') {
		if(Movable(p, d)) {
			MoveH(p, d);
			return true;
		}
	}
	return false;
}

void Move(char direction) {
	if((direction == '<' || direction == '>') && TryMoveH(UpdatePoint(position, direction), direction)) 
		position = UpdatePoint(position, direction);
	if((direction == '^' || direction == 'v') && TryMoveV(UpdatePoint(position, direction), direction)) 
		position = UpdatePoint(position, direction);
}

void DrawMap() {
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH*2; j++) {
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

	DrawMap();
	//while(true) {
	//	system("stty raw");
	//	char c = getc(stdin);
	//	system("stty cooked");
	//	if(c) Move(ToInst(c));
	//	DrawMap();
	//}

	for(int i = 0; i < arrlen(path); i++) {
		Move(path[i]);
		//DrawMap();
		//usleep(50000);
	}

	DrawMap();

	int sum = 0;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH*2; j++) {
			if(map[i][j] == '[') sum += (100 * i) + j;
		}
	}

	printf("Part 2: %d\n", sum);

	return 0;
}
