#ifndef RISKV_TABLE
#define RISKV_TABLE

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define TABLE_MIN_SIZE 8
#define TABLE_LOAD_FACTOR 0.75

typedef struct
{
	char* key;
	uint32_t hash;
	int key_length;
	bool is_int;
	union
	{
		int integer;
		char* str;
	} as;
} Entry;

typedef struct
{
	int size;
	int count;
	Entry* entries;
} Table;

void initTable(Table** table);
uint32_t hashString(char* s, int n);
Entry* findEntry(Entry* entries, int size, char* s, int n, uint32_t hash);
bool findInTable(Table* table, char* s, int* value);
bool addToTable(Table* table, char* key, int n);
bool addStringToTable(Table* table, char* key, char* value);
void freeTable(Table* table);
int getValueFromTable(Table* table, char* key);
char* getStringFromTable(Table* table, char* key);

#endif
