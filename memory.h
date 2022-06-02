#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"
#include "object.h"

#define ALLOCATE(type, count) \
	(type*)reallocate(NULL, 0, sizeof(type) * (count))

#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

#define GROW_CAPACITY(t) ((t) < 8 ? 8 : ((t)*2))
#define GROW_ARRAY(type, ptr, oldCount, newCount) \
	(type*)reallocate(ptr, (oldCount) * sizeof(type), \
			(newCount) * sizeof(type))
#define FREE_ARRAY(type, ptr, oldCount) \
	reallocate(ptr, (oldCount) * sizeof(type), 0)

void* reallocate(void *ptr, size_t oldSize, size_t newSize);
void freeObjects();

#endif
