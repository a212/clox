
#include "chunk.h"
#include "memory.h"

void initChunk(Chunk *chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	initLineArray(&chunk->lines);
	initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
	if (chunk->count >= chunk->capacity) {
		chunk->count = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(chunk->capacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, chunk->count, chunk->capacity);
	}
	chunk->code[chunk->count] = byte;
	writeLineArray(&chunk->lines, line, chunk->count);
	chunk->count++;
}

int addConstant(Chunk *chunk, Value value) {
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}

void freeChunk(Chunk *chunk) {
	FREE_ARRAY(uint8_t, chunk->code, 0);
	freeLineArray(&chunk->lines);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

int getLine(Chunk *chunk, int offset) {
	return getLineArrayLine(&chunk->lines, offset);
}

