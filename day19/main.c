#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define COUNT 400

typedef struct trieItem TrieItem;
struct trieItem {
	char letter;
	TrieItem* next;
};

TrieItem* trie = NULL;

bool ContainsLetter(TrieItem* trie, char letter, int* i) {
	for(*i = 0; *i < arrlen(trie); (*i)++) {
		if(trie[*i].letter == letter) return true;
	}
	return false;
}

void TrieAdd(char* pattern) {
	TrieItem** lTrie = &trie;
	for(int i = 0; i <= strlen(pattern); i++) {
		int index = 0;
		if(ContainsLetter(*lTrie, pattern[i], &index)) {
			lTrie = &((*lTrie)[index].next);
		}
		else { 
			arrput((*lTrie), (TrieItem) { .letter = pattern[i] });
			lTrie = &(*lTrie)[arrlen(*lTrie)-1].next;
		}
	}
}

bool ValidTovel(char* tovel, TrieItem* lTrie) {
	if(*tovel == 0) return true;

	int index = 0;
	int hindex;
	if(ContainsLetter(lTrie, *tovel, &index)) { 
		if(lTrie[index].next == NULL || ContainsLetter(lTrie[index].next, 0, &hindex)) {
			if(ValidTovel(tovel+1, trie)) return true;
		}
		if(ValidTovel(tovel+1, lTrie[index].next)) return true;
	}
	
	return false;
}

long SolveFile(char* fileName) {
	long validTovels = 0;
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	fseek(f, 0, SEEK_SET);
	char* line = NULL;
	size_t lineLen;
	getline(&line, &lineLen, f);
	char* token = strtok(line, ", ");
	while(token) {
		TrieAdd(token);
		token = strtok(NULL, ", \n");
	}

	getline(&line, &lineLen, f);

	for(int i = 0; i < COUNT; i++) {
		getline(&line, &lineLen, f);
		line[strlen(line)-1] = 0;
		if(ValidTovel(line, trie)) validTovels++;
	}
	free(line);

	fclose(f);
	return validTovels;
}

int main() {
	long validTovels = SolveFile("input.txt");

	printf("Part 1: %ld\n", validTovels);
	return 0;
}
