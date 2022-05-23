
#ifndef clox_scanner_h
#define clox_scanner_h

typedef enum {
	TOKEN_EMPTY,
	TOKEN_INVALID,
	TOKEN_WSPACE,
	TOKEN_OP = 7,
	TOKEN_SEP = 8,
	TOKEN_NUMBER = 21,
	TOKEN_IDENT = 31,
	TOKEN_EOF = 39,
} TokenType;

typedef struct {
	TokenType type;
	int length;
	const char* start;
	int line;
} Token;

void initScanner(const char *source);
Token scanToken();

#endif

