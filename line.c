
#include <stdio.h>
#include "line.h"
#include "memory.h"

void initLineArray(LineArray *array) {
	array->count = 0;
	array->capacity = 0;
	array->line = 0;
	array->offset = 0;
	array->lines = NULL;

}

static void writeLineArrayItem(LineArray *array, Line line) {
	if (array->count >= array->capacity) {
		array->count = array->capacity;
		array->capacity = GROW_CAPACITY(array->capacity);
		array->lines = GROW_ARRAY(Line, array->lines, array->count, array->capacity);
	}
	array->lines[array->count++] = line;
	array->line += line.lineOffset;
	array->offset += line.instructionOffset;
}

#define min(a,b) (((a) > (b))?(b):(a))
void writeLineArray(LineArray *array, int line, int offset) {
	while (array->line < line || array->offset < offset) {
		Line it;
		it.instructionOffset = min(offset - array->offset, 255);
		it.lineOffset = min(line - array->line, 255);
		writeLineArrayItem(array, it);
	}
}

int getLineArrayLine(LineArray *array, int offset) {
	int line = 0;
	int offs = 0;
	for (int i = 0; i < array->count; i++) {
		offs += array->lines[i].instructionOffset;
		if (offs > offset) break;
		line += array->lines[i].lineOffset;
	}
	return line;
}

void freeLineArray(LineArray *array) {
	FREE_ARRAY(Line, array->lines, 0);
	initLineArray(array);
}

