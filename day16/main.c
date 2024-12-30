#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define WIDTH 141
#define HEIGHT 141
#define VISITED 0
#define SPACE -1
#define WALL -2
#define MARK -3

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

typedef struct {
	Point p;
	Direction d;
	int value;
} Position;

// Min Heap DS
int GetLeftChildIndex(int parentIndex) { return 2 * parentIndex + 1; }
int GetRightChildIndex(int parentIndex) { return 2 * parentIndex + 2; }
int GetParentIndex(int childIndex) { return (childIndex - 1) / 2; }

bool HasLeftChild(Position* p, int index) { return GetLeftChildIndex(index) < arrlen(p); }
bool HasRightChild(Position* p, int index) { return GetRightChildIndex(index) < arrlen(p); }
bool HasParent(Position* p, int index) { return GetParentIndex(index) >= 0; }

Position LeftChild(Position* p, int index) { return p[GetLeftChildIndex(index)]; }
Position RightChild(Position* p, int index) { return p[GetRightChildIndex(index)]; }
Position Parent(Position* p, int index) { return p[GetParentIndex(index)]; }

void QSwap(Position* p, int a, int b) {
	Position temp = p[a];
	p[a] = p[b];
	p[b] = temp;
}

bool QEmpty(Position* p) { return arrlen(p) == 0; }

Position QPoll(Position* p) {
	assert(!QEmpty(p));

	Position root = p[0];
	arrdelswap(p, 0);
	// heapify down
	int index = 0;
	while(HasLeftChild(p, index)) {
		int smallerChildIndex = GetLeftChildIndex(index);
		if(HasRightChild(p, index) && RightChild(p, index).value < LeftChild(p, index).value) {
			smallerChildIndex = GetRightChildIndex(index);
		}
		if(p[index].value < p[smallerChildIndex].value) break;
		QSwap(p, index, smallerChildIndex);
		index = smallerChildIndex;
	}
	return root;
}

Position* QAdd(Position* p, Position p1) {
	arrput(p, p1);
	// heapify up
	int index = arrlen(p) - 1;
	while(HasParent(p, index) && Parent(p, index).value > p[index].value) {
		QSwap(p, GetParentIndex(index), index);
		index = GetParentIndex(index);
	}
	return p;
}
// --------------------

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

Position NewPosition(Point p, Direction d, int value) {
	Position np = { .p = p, .d = d, .value = value };
	map[p.i][p.j] = VISITED;
	return np;
}

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

typedef struct {
	Point key;
	Position* positions;
	Position* source; //array of source position
} PositionDict;
PositionDict* positions = NULL;

Position  GetChipestPosition(Position* p) {
	Position pc = p[0];
	for(int i = 1; i < arrlen(p); i++) {
		if(pc.value > p[i].value) pc = p[i];
	}
	return pc;
}

void Dijkstra() {
	Position* q = NULL;
	
	Position p = NewPosition(start, RIGHT, 0);
	q = QAdd(q, p);
	while(!QEmpty(q)) {
		p = QPoll(q);

		for(int d = 0; d <= LEFT; d++) {
			Point np = GetNextPoint(p.p, d);
			if(map[np.i][np.j] == SPACE) {
				Position position;
				if(d != p.d) position = NewPosition(p.p, d, p.value+1000);
				else position = NewPosition(np, d, p.value+1);

				Position* possible = hmgets(positions, position.p).positions;
				Position* source = hmgets(positions, position.p).source;
				arrput(possible, position);
				if(d == p.d) arrput(source, p);
				PositionDict pd = { .key = position.p, .positions = possible, .source = source };
				hmputs(positions, pd);

				q = QAdd(q, position); // rewrite queue reference
			}
			else if(map[np.i][np.j] >= 0) { // if space already visited
				// check if there is position on that point which is one step biger, if so link them
				Position* possible = hmgets(positions, np).positions;
				Position* source = hmgets(positions, np).source;
				if(possible) {
					for(int i = 0; i < arrlen(possible); i++) {
						if(possible[i].value - p.value == 1 && possible[i].d == p.d) {
							arrput(source, p);
							PositionDict pd = { .key = np, .positions = possible, .source = source };
							hmputs(positions, pd);
						}
					}
				}
			}
		}
	}
}

PositionDict* shortestPath = NULL;
void MarkShortestPath() {
	Position* q = NULL;
	Position p = GetChipestPosition(hmgets(positions, end).positions);
	q = QAdd(q, p);
	PositionDict pd = { .key = end, .positions = 1, .source = 1 };
	hmputs(shortestPath, pd);

	while(!QEmpty(q)) {
		p = QPoll(q);

		pd = (PositionDict) { .key = p.p, .positions = 1, .source = 1 };
		hmputs(shortestPath, pd);
		Position* source = hmgets(positions, p.p).source;
		for(int i = 0; i < arrlen(source); i++) {
			q = QAdd(q, source[i]);
		}
	}
}

int main() {
	ParseFile("input.txt");
	Dijkstra();
	printf("Part 1: %d\n", GetChipestPosition(hmgets(positions, end).positions).value);
	//for(int i = 0; i < HEIGHT; i++) {
	//	for(int j = 0; j < WIDTH; j++) {
	//		Point p = { .i = i, .j = j };
	//		Position* pos = hmgets(positions, p).positions;
	//		if(map[i][j] == WALL) printf("#### ");
	//		else if (pos) {
	//			printf("%d ", GetChipestPosition(pos).value);
	//		}
	//	}
	//	printf("\n");
	//}
	
	MarkShortestPath();
	printf("Part 2: %ld\n", hmlen(shortestPath));
	//for(int i = 0; i < HEIGHT; i++) {
	//	for(int j = 0; j < WIDTH; j++) {
	//		Point p = { .i = i, .j = j };
	//		if(map[i][j] == WALL) printf("#");
	//		else if(hmgets(shortestPath, p).positions) printf("O");
	//		else printf(".");
	//	}
	//	printf("\n");
	//}
	return 0;
}
