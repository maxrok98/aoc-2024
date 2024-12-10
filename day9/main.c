#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define FILE_LENGTH 19999

char* ParseFile(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	char* line = NULL;
	size_t lineLen;
	getline(&line, &lineLen, f);

	fclose(f);
	return line;
}

int ctoi(char a) {
	return (int)(a - 48);
}

int* ExpandMap(char* diskMap) {
	int* diskBlocks = NULL;
	int i = 0;
	int fileIndex = 0;
	while(true) {
		if((i >= FILE_LENGTH)) break;
		for(int j = 0; j < ctoi(diskMap[i]); j++) {
			arrput(diskBlocks, fileIndex);
		}
		fileIndex++; i++;

		if(i >= FILE_LENGTH) break;
		for(int j = 0; j < ctoi(diskMap[i]); j++) {
			arrput(diskBlocks, -1);
		}
		i++;
	}
	
	return diskBlocks;
}

void OptimizeBlocks(int* diskBlocks) {
	bool replacedBlock = true;
	for(int i = arrlen(diskBlocks)-1; replacedBlock; i--) {
		while(diskBlocks[i] == -1) i--; //skip all dots
		for(int j = 0; j < i; j++) {
			if(diskBlocks[j] == -1) {
				diskBlocks[j] = diskBlocks[i];
				diskBlocks[i] = -1;
				replacedBlock = true;
				break;
			}
			replacedBlock = false;
		}
	}
}

int FindNextFileBlock(int* diskBlocks, int start, int* size) {
	int beginning, end;
	while(diskBlocks[start] == -1) start--; //skip all dots
	beginning = end = start;
	while(beginning > 0 && diskBlocks[beginning-1] == diskBlocks[end]) beginning--;
	*size = (end - beginning) + 1;
	return beginning;
}

bool FindAvailableSpot(int* diskBlock, int* availableSpotIndex, int size, int cap) {
	for(int i = 0; i < cap; i++) {
		if(diskBlock[i] == -1) {
			int j = i;
			while(diskBlock[j] == -1) j++;
			if((j - i) >= size) {
				*availableSpotIndex = i;
				return true;
			}
			i = j;
		}
	}
	
	return false;
}

long Checksum(int* diskBlocks) {
	long sum = 0;
	for(int i = 0; i < arrlen(diskBlocks); i++) {
		if(diskBlocks[i] != -1) {
			sum += i * diskBlocks[i];
		}
	}
	return sum;
}

void OptimizeBlocks2(int* diskBlocks) {
	int size;
	int blockBeginning = FindNextFileBlock(diskBlocks, arrlen(diskBlocks)-1, &size);
	while(blockBeginning > 0) {
		int availableSpotIndex = 0;
		if(FindAvailableSpot(diskBlocks, &availableSpotIndex, size, blockBeginning)) { // <---- do not stop if not found
			memcpy(diskBlocks+availableSpotIndex, diskBlocks+blockBeginning, sizeof(int)*size);
			memset(diskBlocks+blockBeginning, -1, sizeof(int)*size);
		}

		blockBeginning = FindNextFileBlock(diskBlocks, blockBeginning-1, &size);
	}
}

int main() {
	char* diskMap = ParseFile("input.txt");
	int* diskBlocks = ExpandMap(diskMap);
	OptimizeBlocks(diskBlocks);
	printf("Part 1: %ld\n", Checksum(diskBlocks));

	int* diskBlocks2 = ExpandMap(diskMap);
	OptimizeBlocks2(diskBlocks2);
	printf("Part 2: %ld\n", Checksum(diskBlocks2));

	//4 333 801 636 191 too low
	
	return 0;
}
