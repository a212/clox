
#ifndef clox_line_h
#define clox_line_h

#include "common.h"

typedef struct {
	uint8_t instructionOffset;
	uint8_t lineOffset;
} Line;

typedef struct {
	int capacity;
	int count;
	Line *lines;
	int line;
	int offset;
} LineArray;

void initLineArray(LineArray *array);
void writeLineArray(LineArray *array, int line, int offset);
int getLineArrayLine(LineArray *array, int offset);
void freeLineArray(LineArray *array);

#endif

