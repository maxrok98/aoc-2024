#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

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

void printfArr(Position* p) {
	for(int i = 0; i < arrlen(p); i++) {
		printf("%d ", p[i].value);
	}
	printf("\n");
}

int main() {
	Position* a = NULL;
	Position p = {0};
	p.value = 10;
	arrput(a, p);
	p.value = 15;
	arrput(a, p);
	p.value = 20;
	arrput(a, p);
	p.value = 17;
	arrput(a, p);

	p.value = 8;
	printfArr(a);
	a = QAdd(a, p);
	printfArr(a);
	
	return 0;
}
