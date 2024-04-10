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
	int32_t key_length;
	bool is_int;
	union
	{
		int32_t integer;
		char* str;
	} as;
} Entry;

typedef struct
{
	int32_t size;
	int32_t count;
	Entry* entries;
} Table;

void initTable(Table** table);
uint32_t hashString(char* s, int32_t n);
Entry* findEntry(Entry* entries, int32_t size, char* s, int32_t n, uint32_t hash);
bool findInTable(Table* table, char* s, int32_t* value);
bool addToTable(Table* table, char* key, int32_t n);
bool addValueToTable(Table* table, char* key, int32_t n);
bool addStringToTable(Table* table, char* key, char* value);
void freeTable(Table* table);
int32_t getValueFromTable(Table* table, char* key);
char* getStringFromTable(Table* table, char* key);

#endif
