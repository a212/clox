
#include <stdio.h>
#include "value.h"
#include "memory.h"

void initValueArray(ValueArray *array) {
	array->count = 0;
	array->capacity = 0;
	array->values = NULL;

}

void writeValueArray(ValueArray *array, Value value) {
	if (array->count >= array->capacity) {
		array->count = array->capacity;
		array->capacity = GROW_CAPACITY(array->capacity);
		array->values = GROW_ARRAY(Value, array->values, array->count, array->capacity);
	}
	array->values[array->count++] = value;
}

void printValue(Value value) {
	switch (value.type) {
		case VAL_BOOL:
			printf("%s", AS_BOOL(value) ? "true" : "false");
			break;
		case VAL_NIL:
			printf("nil");
			break;
		case VAL_NUMBER:
			printf("%g", AS_NUMBER(value));
			break;
	}
}

void freeValueArray(ValueArray *array) {
	FREE_ARRAY(Value, array->values, 0);
	initValueArray(array);
}

