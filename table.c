
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table* table) {
	table->count = 0;
	table->capacity = 0;
	table->entries = NULL;
}

void freeTable(Table *table) {
	FREE_ARRAY(Entry, table->entries, table->capacity);
	initTable(table);
}

static Entry* findEntry(Entry *entries, int capacity, ObjString* key) {
	int index = key->hash % capacity;
	for (;;) {
		Entry* entry = &entries[index];
		if (entry->key == key || entry->key == NULL) {
			return entry;
		}
		index = (index + 1) % capacity;
	}
}

static void adjustCapacity(Table* table, int capacity) {
	Entry *entries = ALLOCATE(Entry, capacity);
	for (int i = 0; i < capacity; i++) {
		entries[i].key = NULL;
		entries[i].value = NIL_VAL;
	}
	for (int i = 0; i < table->capacity; i++) {
		Entry* entry = &table->entries[i];
		if (entry->key == NULL) continue;
		Entry* dest = findEntry(entries, capacity, entry->key);
		dest->key = entry->key;
		dest->value = entry->value;
	}
	FREE_ARRAY(Entry, table->entries, table->capacity);
	table->entries = entries;
	table->capacity = capacity;
}

bool tableGet(Table* table, ObjString* key, Value* value) {
	if (table->count == 0) return false;
	Entry* entry = findEntry(table->entries, table->capacity, key);
	if (entry->key != key) return false;
	if (value) *value = entry->value;
	return true;
}

bool tableSet(Table* table, ObjString* key, Value value) {
	if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
		int capacity = GROW_CAPACITY(table->capacity);
		adjustCapacity(table, capacity);
	}
	Entry* entry = findEntry(table->entries, table->capacity, key);
	bool isNewKey = entry->key == NULL;
	if (isNewKey) table->count++;
	entry->key = key;
	entry->value = value;
	return isNewKey;
}

bool tableDelete(Table* table, ObjString* key) {
	if (table->count == 0) return false;
	Entry* entry = findEntry(table->entries, table->capacity, key);
	if (entry->key != key) return false;
	size_t index = entry - table->entries;
	size_t next = index;
	for (;;) {
		size_t next = (next + 1) % table->capacity;
		if (table->entries[next].key == NULL) break;
		int nextIdx = table->entries[next].key->hash % table->capacity;
		if (nextIdx <= index || nextIdx > next) {
			*entry = table->entries[next];
			entry = &table->entries[next];
			index = next;
		}

	}
	entry->key = NULL;
	entry->value = NIL_VAL;
}

void tableAddAll(Table* from, Table* to) {
	for (int i = 0; i < from->capacity; i++) {
		Entry* entry = &from->entries[i];
		if (entry->key) {
			tableSet(to, entry->key, entry->value);
		}
	}
}
