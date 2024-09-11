#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "table.h"
#include "common.h"

void initTable(Table** table)
{
	*table = (Table*)malloc(sizeof(Table));
	(*table)->size = 0;
	(*table)->count = 0;
	(*table)->entries = NULL; 
}

void freeTable(Table* table)
{
	free(table->entries);
	initTable(&table);
}

uint32_t hashString(char* s, int32_t n)
{
	uint32_t hash = 2166136261u;
	for (int32_t i = 0; i < n; i++)
	{
		hash ^= s[i];
		hash *= 16777619;
	}

	return hash;
}

Entry* findEntry(Entry* entries, int32_t size, char* s, int32_t n, uint32_t hash)
{
	if(size == 0)
		return NULL;
	uint32_t index = hash % size;
	for (;;)
	{
		Entry* entry = &entries[index];
		if (entry == NULL || entry->key == NULL)
		{
			return entry;
		}

		else if(entry->key_length == n && entry->hash == hash && memcmp(entry->key, s, n) == 0)
			return entry;

		index = (index + 1) % size;
	}
}

bool findStringInTable(Table* table, char* s, char** val)
{
	int32_t n = strlen(s);
	uint32_t hash = hashString(s, n);	
	
	Entry* entry = findEntry(table->entries, table->size, s, n, hash);
	if(entry->key == NULL)
		return false;
	
	(*val) = entry->as.str;
	return true;

}

bool findInTable(Table* table, char* s, int32_t* val)
{
	int32_t n = strlen(s);
	uint32_t hash = hashString(s, n);	
	
	Entry* entry = findEntry(table->entries, table->size, s, n, hash);
	if(entry->key == NULL)
		return false;

	*val = entry->as.integer;
	return true;
}

int32_t getValueFromTable(Table* table, char* key)
{
	int32_t n = strlen(key);

	Entry* entry = findEntry(table->entries, table->size, key, n, hashString(key, n));
	if(entry->key == NULL)
		return -1;
	return entry->as.integer;
}

char* getStringFromTable(Table* table, char* key)
{
	char* str;
	if(findStringInTable(table, key, &str))
			return str;

	return NULL;
}

static void adjustSize(Table* table, int32_t capacity)
{
	Entry* entries = (Entry*)malloc(capacity * sizeof(Entry));
	for(int32_t i = 0; i < capacity; i++)
	{
		entries[i].key = NULL;
		entries[i].as.integer = 0;
		entries[i].as.str = NULL;
	}

	for(int32_t i = 0; i < table->size; i++)
	{
		Entry* entry = &table->entries[i];
		if (entry->key == NULL) continue;

		Entry* dest = findEntry(entries, capacity, entry->key, entry->key_length, entry->hash);
		dest->key = entry->key;
		dest->key_length = entry->key_length;
		dest->hash = entry->hash;
		dest->as.integer = entry->as.integer;
		dest->as.str = entry->as.str;
	}

	free(table->entries);
	table->size = capacity;
	table->entries = entries;
}

bool addToTable(Table* table, char* s, int32_t n)
{
	if(table->count + 1 > table->size * TABLE_LOAD_FACTOR)
	{
		int32_t capacity = GROW_LIST(table->size);
		adjustSize(table, capacity);
	}
	
	uint32_t hash = hashString(s, n);
	Entry* entry = findEntry(table->entries, table->size, s, n, hash);
	bool is_new_entry = (entry->key == NULL); 
	if (is_new_entry) table->count++;

	entry->key = s;
	entry->hash = hash;
	entry->as.integer++;
	entry->key_length = n;
	return is_new_entry;
}

bool addValueToTable(Table* table, char* key, int32_t value)
{
	int32_t key_length = strlen(key);
	if(table->count + 1 > table->size * TABLE_LOAD_FACTOR)
	{
		int32_t capacity = GROW_LIST(table->size);
		adjustSize(table, capacity);
	}

	uint32_t hash = hashString(key, key_length);
	Entry* entry = findEntry(table->entries, table->size, key, key_length, hash);
	bool is_new_entry = (entry->key == NULL);
	if (is_new_entry) table->count++;

	entry->key = key;
	entry->hash = hash;
	entry->as.integer = value;
	entry->key_length = key_length;

	return is_new_entry;
}

bool addStringToTable(Table* table, char* key, char* value)
{
	int32_t key_length = strlen(key);
	if(table->count + 1 > table->size * TABLE_LOAD_FACTOR)
	{
		int32_t capacity = GROW_LIST(table->size);
		adjustSize(table, capacity);
	}
	uint32_t hash = hashString(key, key_length);
	Entry* entry = findEntry(table->entries, table->size, key, key_length, hash);
	bool is_new_entry = (entry->key == NULL);
	if (is_new_entry) table->count++;

	entry->key = key;
	entry->hash = hash;
	entry->as.str = value;
	entry->key_length = key_length;

	return is_new_entry;
}
