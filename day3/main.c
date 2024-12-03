#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

int LoadFile(char* fileName, char** content) {
	FILE* f = fopen(fileName, "rb");
	if(f == NULL) {
		printf("Error: %s file not found!\n", fileName);
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* fileContent = malloc(fsize + 1);
	fread(fileContent, fsize, 1, f);
	fclose(f);

	fileContent[fsize] = 0;
	*content = fileContent;
	return fsize;;
}

typedef enum TokenType {
	L_PAREN,
	R_PAREN,
	INTEGER,
	COMMA,
	MUL,
	GARBAGE,
	END
} TokenType;

typedef struct Token {
	TokenType tokenType;
	int value;
} Token;

typedef struct {
	char* string;
	int length;

	/* window indexes */
	int start;
	int end;
} Tokenizer;

void AdvanceCursor(Tokenizer* tokenizer) {
	tokenizer->start = ++tokenizer->end;
}

int ToInteger(char* string, int start, int end) {
	int num = 0;
	for(int i = start; i <= end; i++) {
		num = num * 10 + (string[i] - 48);
	}
	return num;
}

Token nextToken(Tokenizer* tokenizer) {
	assert(tokenizer->start == tokenizer->end);
	Token token;
	while(true) {
		if(tokenizer->start >= tokenizer->length) { token = (Token) { .tokenType = END }; break; }
		if(tokenizer->string[tokenizer->start] == '(') { token = (Token) { .tokenType = L_PAREN }; break; }
		if(tokenizer->string[tokenizer->start] == ')') { token = (Token) { .tokenType = R_PAREN }; break; }
		if(tokenizer->string[tokenizer->start] == ',') { token = (Token) { .tokenType = COMMA   }; break; }
		if(isdigit(tokenizer->string[tokenizer->start])) {
			while (isdigit(tokenizer->string[tokenizer->end])) { tokenizer->end++; }
			tokenizer->end--;
			int num = ToInteger(tokenizer->string, tokenizer->start, tokenizer->end);
			token = (Token) { .tokenType = INTEGER, .value = num }; break;
		}
		if(tokenizer->string[tokenizer->start] == 'm' && tokenizer->start < tokenizer->length-2 ) { 
			if(tokenizer->string[tokenizer->start+1] == 'u' && tokenizer->string[tokenizer->start+2] == 'l') {
				token = (Token) { .tokenType = MUL };
				tokenizer->end += 2;
				break;
			}
		}
		token = (Token) { .tokenType = GARBAGE }; break;
	}
	AdvanceCursor(tokenizer);
	return token;
}

int main() {
	char* fileContent;
	int length = LoadFile("input.txt", &fileContent);

	Tokenizer tokenizer = {
		.string = fileContent,
		.length = length,
		.start = 0,
		.end = 0
	};

	int sum = 0;
	Token token;
	while((token = nextToken(&tokenizer)).tokenType != END) {
		Token num1;
		Token num2;
		if(token.tokenType != MUL) continue;
		if(nextToken(&tokenizer).tokenType != L_PAREN) continue;
		if((num1 = nextToken(&tokenizer)).tokenType != INTEGER) continue;
		if(nextToken(&tokenizer).tokenType != COMMA) continue;
		if((num2 = nextToken(&tokenizer)).tokenType != INTEGER) continue;
		if(nextToken(&tokenizer).tokenType != R_PAREN) continue;
		sum += num1.value * num2.value;
	}

	printf("Part 1: %d\n", sum);
	return 0;
}
