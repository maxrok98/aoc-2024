#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define DROP_COUNT 3450
#define WIDTH 71
#define HEIGHT 71
#define WALL -1
#define VISITED -2

int map[HEIGHT][WIDTH] = {0};

typedef struct {
	int i;
	int j;
} Point;

Point start = { .i = 0, .j = 0 };
Point end = { .i = HEIGHT - 1, .j = WIDTH - 1 };

Point points[DROP_COUNT] = {0};

void ParseFile(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}
	fseek(f, 0, SEEK_SET);

	char* line = NULL;
	size_t lineLen;
	for (int i = 0; i < DROP_COUNT; i++) {
	 	getline(&line, &lineLen, f);
		char* token = strtok(line, ",");
		points[i].j = atoi(token);
		token = strtok(NULL, "\n");
		points[i].i = atoi(token);
	}
	free(line);

	fclose(f);
}

//typedef struct {
//	Point key;
//	int value;
//} KiloDrop;
//KiloDrop kiloDrop = NULL;

// Min Heap DS
int GetLeftChildIndex(int parentIndex) { return 2 * parentIndex + 1; }
int GetRightChildIndex(int parentIndex) { return 2 * parentIndex + 2; }
int GetParentIndex(int childIndex) { return (childIndex - 1) / 2; }

bool HasLeftChild(Point* p, int index) { return GetLeftChildIndex(index) < arrlen(p); }
bool HasRightChild(Point* p, int index) { return GetRightChildIndex(index) < arrlen(p); }
bool HasParent(Point* p, int index) { return GetParentIndex(index) >= 0; }

Point LeftChild(Point* p, int index) { return p[GetLeftChildIndex(index)]; }
Point RightChild(Point* p, int index) { return p[GetRightChildIndex(index)]; }
Point Parent(Point* p, int index) { return p[GetParentIndex(index)]; }

void QSwap(Point* p, int a, int b) {
	Point temp = p[a];
	p[a] = p[b];
	p[b] = temp;
}

bool QEmpty(Point* p) { return arrlen(p) == 0; }

int MapValue(Point p) {
	return map[p.i][p.j];
}

Point QPoll(Point* p) {
	assert(!QEmpty(p));

	Point root = p[0];
	arrdelswap(p, 0);
	// heapify down
	int index = 0;
	while(HasLeftChild(p, index)) {
		int smallerChildIndex = GetLeftChildIndex(index);
		if(HasRightChild(p, index) && MapValue(RightChild(p, index)) < MapValue(LeftChild(p, index))) {
			smallerChildIndex = GetRightChildIndex(index);
		}
		if(MapValue(p[index]) < MapValue(p[smallerChildIndex])) break;
		QSwap(p, index, smallerChildIndex);
		index = smallerChildIndex;
	}
	return root;
}

Point* QAdd(Point* p, Point p1) {
	arrput(p, p1);
	// heapify up
	int index = arrlen(p) - 1;
	while(HasParent(p, index) && MapValue(Parent(p, index)) > MapValue(p[index])) {
		QSwap(p, GetParentIndex(index), index);
		index = GetParentIndex(index);
	}
	return p;
}
// --------------------

void Dijkstra() {
	Point* q = NULL;
	
	Point p = start;
	q = QAdd(q, p);
	while(!QEmpty(q)) {
		p = QPoll(q);

		bool valid_top = p.i != 0, valid_bottom = p.i != HEIGHT-1;
		bool valid_left = p.j != 0, valid_right = p.j != WIDTH-1; 

		if(valid_top && map[p.i-1][p.j] == 0) { map[p.i-1][p.j] = MapValue(p)+1; q = QAdd(q, (Point) { .i=p.i-1, .j=p.j }); }

		if(valid_right && map[p.i][p.j+1] == 0) { map[p.i][p.j+1] = MapValue(p)+1; q = QAdd(q, (Point) { .i=p.i, .j=p.j+1 }); }

		if(valid_bottom && map[p.i+1][p.j] == 0) { map[p.i+1][p.j] = MapValue(p)+1; q = QAdd(q, (Point) { .i=p.i+1, .j=p.j }); }

		if(valid_left && map[p.i][p.j-1] == 0) { map[p.i][p.j-1] = MapValue(p)+1; q = QAdd(q, (Point) { .i=p.i, .j=p.j-1 }); }
	}
}

int main() {
	ParseFile("input.txt");
	for(int i = 0; i < 1024; i++) {
		map[points[i].i][points[i].j] = WALL;
	}

	Dijkstra();

	printf("Part 1: %d\n", MapValue(end));

	int j = 1025;
	for(; j < DROP_COUNT; j++) {
		memset(&map, 0, sizeof(int)*HEIGHT*WIDTH);
		for(int i = 0; i <= j; i++) {
			map[points[i].i][points[i].j] = WALL;
		}
		Dijkstra();
		if(MapValue(end) <= 0) break;
	}

	printf("Part 2: %d,%d\n", points[j].j, points[j].i);
	
	return 0;
}
