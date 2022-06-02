
#include <stdlib.h>

#include "memory.h"
#include "vm.h"

void* reallocate(void *ptr, size_t oldSize, size_t newSize) {
	if (newSize == 0) {
		free(ptr);
		return NULL;
	}
	void* res = realloc(ptr, newSize);
	if (res == NULL) exit(1);
	return res;
}

static void freeObject(Obj* object) {
	switch (object->type) {
		case OBJ_STRING: {
			ObjString* string = (ObjString*)object;
			FREE_ARRAY(char, string->chars, string->length + 1);
			FREE(ObjString, object);
			break;
		}
	}
}

void freeObjects() {
	for (Obj* obj = vm.objects; obj;) {
		Obj* next = obj->next;
		freeObject(obj);
		obj = next;
	}
}
