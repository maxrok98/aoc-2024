#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define COUNT 320

typedef struct {
	double ax;
	double ay;
	double bx;
	double by;
	double px;
	double py;
} Machine;

Machine machines[COUNT];

void ParseFile(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}

	fseek(f, 0, SEEK_SET);
	char* line = NULL;
	size_t lineLen;
	for (int i = 0; i < COUNT; i++) {
		getline(&line, &lineLen, f);
		char* token = strtok(line, "+");
		token = strtok(NULL, ",");
		machines[i].ax = atoi(token);
		token = strtok(NULL, "+");
		token = strtok(NULL, "\n");
		machines[i].ay = atoi(token);

		getline(&line, &lineLen, f);
		token = strtok(line, "+");
		token = strtok(NULL, ",");
		machines[i].bx = atoi(token);
		token = strtok(NULL, "+");
		token = strtok(NULL, "\n");
		machines[i].by = atoi(token);

		getline(&line, &lineLen, f);
		token = strtok(line, "=");
		token = strtok(NULL, ",");
		machines[i].px = atoi(token);
		token = strtok(NULL, "=");
		token = strtok(NULL, "\n");
		machines[i].py = atoi(token);

		getline(&line, &lineLen, f);
	}
	free(line);

	fclose(f);
}

typedef struct {
	double a;
	double b;
} Solution;

Solution Solve(Machine m) {
	Solution s = {0};
	s.b = (m.py * m.ax - m.px * m.ay) / (m.ax * m.by - m.ay * m.bx);
	s.a = (m.px - m.bx * s.b) / m.ax;

	return s;
}

int main() {
	ParseFile("input.txt");

	long sum = 0;
	for(int i = 0; i < COUNT; i++) {
		Solution s = Solve(machines[i]);
		if(floor(s.a) == s.a && floor(s.b) == s.b) {
			sum += (s.a * 3) + s.b;
		}
	}

	printf("Part 1: %ld\n", sum);

	sum = 0;
	for(int i = 0; i < COUNT; i++) {
		machines[i].px += 10000000000000;
		machines[i].py += 10000000000000;
		Solution s = Solve(machines[i]);
		printf("%f %f\n", s.a, s.b);
		if(floor(s.a) == s.a && floor(s.b) == s.b) {
			sum += (s.a * 3) + s.b;
		}
	}

	printf("Part 2: %ld\n", sum);
	
	return 0;
}
