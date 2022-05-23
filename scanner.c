
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
	const char* start;
	const char* current;
	int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

Token scanToken() {
	Token token;
	token.type = TOKEN_EMPTY;
	for (; token.type != TOKEN_EOF; scanner.current++) {
		char symb = *scanner.current;
		TokenType tokenNew = TOKEN_INVALID;
		switch(symb) {
		case '\0':
			tokenNew = TOKEN_EOF;
			break;
		case '\n':
			scanner.line++;
		case ' ':
		case '\t':
		case '\r':
			tokenNew = TOKEN_WSPACE;
			break;
		case '+':
		case '-':
		case '*':
		case '/':
			tokenNew = TOKEN_OP;
			break;
		case ';':
			tokenNew = TOKEN_SEP;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			tokenNew = TOKEN_NUMBER;
			break;
	        default:
			if (symb >= 'a' && symb <= 'z' || symb >= 'A' && symb <= 'Z' || symb == '_') {
				tokenNew = TOKEN_IDENT;
			}
			break;
		}
		if (token.type == tokenNew) continue;
		if (token.type == TOKEN_IDENT && tokenNew == TOKEN_NUMBER) continue;
		if (token.type != TOKEN_EMPTY) break;
		if (tokenNew == TOKEN_WSPACE) continue;
		token.type = tokenNew;
		token.start = scanner.current;
		token.line = scanner.line;
	}
	token.length = scanner.current - token.start;
	return token;
}