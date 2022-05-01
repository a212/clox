
#include <stdio.h>
#include "debug.h"
#include "value.h"
#include "line.h"

void disassembleChunk(Chunk *chunk, const char *name) {
	printf("== %s ==\n", name);
	int prevLine = 0;
	for (int offset = 0; offset < chunk->count;) {
		offset = disassembleInstruction(chunk, offset, &prevLine);
	}
}

static int simpleInstruction(const char *name, int offset) {
	printf("%s\n", name);
	return offset + 1;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
	uint8_t constant = chunk->code[offset + 1];
	printf("%-16s %4d '", name, constant);
	printValue(chunk->constants.values[constant]);
	printf("'\n");
	return offset + 2;
}

int disassembleInstruction(Chunk *chunk, int offset, int *prevLine) {
	printf("%04d ", offset);
	int line = getLine(chunk, offset);
	if (offset > 0 && line == *prevLine) {
		printf("   | ");
	} else {
		printf("%4d ", line);
		*prevLine = line;
	}
	uint8_t instruction = chunk->code[offset];
	switch (instruction) {
		case OP_CONSTANT:
			return constantInstruction("OP_CONSTANT", chunk, offset);
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		default:
			printf("Unknown opcode %d\n", instruction);
			return offset + 1;
	}
}

