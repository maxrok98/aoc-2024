#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "ctype.h"

typedef struct {
	long A;
	long B;
	long C;

	int PC;
} CPU;

typedef enum {
	adv = 0,
	bxl = 1,
	bst = 2,
	jnz = 3,
	bxc = 4,
	out = 5,
	bdv = 6,
	cdv = 7
} Instruction;

long Pow(long x,long n) {
	long number = 1;
	for (int i = 0; i < n; ++i)
		number *= x;
	return number;
}

CPU Emulate(CPU cpu, int* program, int** outbuffer) {
	while(true) {
		if(cpu.PC < 0 || cpu.PC >= arrlen(program)-1) break;

		long literal = program[cpu.PC+1];
		long combo = literal;
		if(literal == 4) combo = cpu.A;
		else if(literal == 5) combo = cpu.B;
		else if(literal == 6) combo = cpu.C;

		switch(program[cpu.PC]) {
			case adv:
				cpu.A /= Pow(2, combo);
				break;
			case bxl:
				cpu.B ^= literal;
				break;
			case bst:
				cpu.B = combo % 8;
				break;
			case jnz:
				if(cpu.A) {
					cpu.PC = literal;
					continue;
				}
				break;
			case bxc:
				cpu.B ^= cpu.C;
				break;
			case out:
				arrput(*outbuffer, combo % 8);
				break;
			case bdv:
				cpu.B = cpu.A / Pow(2, combo);
				break;
			case cdv:
				cpu.C = cpu.A / Pow(2, combo);
				break;
		}
		cpu.PC += 2;
	}
	return cpu;
}

void ParseFile(char* fileName, CPU* cpu, int** program) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	fseek(f, 0, SEEK_SET);
	char* line = NULL;
	size_t lineLen;

	getline(&line, &lineLen, f);
	char* token = strtok(line, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, "\n");
	cpu->A = atoi(token);

	getline(&line, &lineLen, f);
	token = strtok(line, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, "\n");
	cpu->B = atoi(token);

	getline(&line, &lineLen, f);
	token = strtok(line, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, "\n");
	cpu->C = atoi(token);

	getline(&line, &lineLen, f);

	getline(&line, &lineLen, f);
	token = strtok(line, " ");
	token = strtok(NULL, ",");
	while(token) {
		arrput(*program, atoi(token));
		token = strtok(NULL, ",");
	}

	free(line);

	fclose(f);
}

long findNumber(long res, long number) {
	long result;
	bool set = false;
	number = number << 3;
	for(long a = 0; a <= 7; a++) {
		number &= ~((long)7);
		number |= a;

		long b = a;
		b ^= 2;
		long c = (number >> b);
		b ^= 3;
		b ^= c;
		if((b & 7) == res) { result = a; set = true; }
	}
	if(!set) assert(false);
	return result;
}

bool FindNumber(long res, long number, int count, long* result) {
	int localcount = 0;
	number = number << 3;
	for(long a = 0; a <= 7; a++) {
		number &= ~((long)7);
		number |= a;

		long b = a;
		b ^= 2;
		long c = (number >> b);
		b ^= 3;
		b ^= c;
		if((b & 7) == res) {
			*result = a;
			localcount++;
			if(localcount == count) return true;
		}
	}
	return false;
}

long suposedNumber = 0;
bool ConstructNumber(int* program, int index, long number) {
	if(index < 0) { suposedNumber = number; return true; }
	int count = 0;
	long a;
	bool found;
	do {
		long ncopy = number;
		count++;

		found = FindNumber(program[index], ncopy, count, &a);
		if(!found) break;
		ncopy = ncopy << 3;
		ncopy |= a;

 		if(ConstructNumber(program, index-1, ncopy)) return true;

	} while(found);
	return false;
}

int main() {
	CPU cpu = {0};
	int* program = NULL;
	ParseFile("input.txt", &cpu, &program);
	int* outbuffer = NULL;
	cpu = Emulate(cpu, program, &outbuffer);

	printf("Part 1: ");
	for(int i = 0; i < arrlen(outbuffer); i++) {
		printf("%d", outbuffer[i]);
		if(i != arrlen(outbuffer)-1) printf(",");
	}
	arrfree(outbuffer);
	printf("\n");


	// this aproach did not work because there are multiple compination of number and program[i] that lead to a but some of those compination also leade to dead end which not allow to find new a, so iterating over possible combination needed
	//long number = 0;
	//for(int i = arrlen(program)-1; i >= 0; i--) {
	//	//if(i == arrlen(program)-1-6) break;
	//	long a = findNumber(program[i], number);
	//	number = number << 3;
	//	number |= a;
	//}
	//cpu.A = number;
	cpu = (CPU) {0};
	ConstructNumber(program, arrlen(program)-1, 0);
	cpu.A = suposedNumber;

	outbuffer = NULL;
	cpu = Emulate(cpu, program, &outbuffer);

	printf("Part 2 output: ");
	for(int i = 0; i < arrlen(outbuffer); i++) {
		printf("%d", outbuffer[i]);
		if(i != arrlen(outbuffer)-1) printf(",");
	}
	arrfree(outbuffer);
	printf("\n");


	printf("Part 2: %ld\n", suposedNumber);
	
	return 0;
}
