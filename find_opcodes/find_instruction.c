#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "table.h"
#include "huffman_algo.h"

Table table;

int cmp(const void* a, const void* b)
{
	return ((Entry*)b)->value - ((Entry*)a)->value;
}

bool isAlphanumeric(char c)
{
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '.') || (c >= '0' && c <= '9') || (c == '_'))
		return true;
	
	return false;
}

void validAlphanumeric(char* s, int* index)
{
	while((s[*index] != ' ') && (s[*index] != '\n') && (s[*index] != '\t') && (s[*index] != '\0') && isAlphanumeric(s[*index]))
	{
		(*index)++;
	}
}

void parseLine(char* s)
{
	int i = 0;
	while(s[i] != '\0' && s[i] != '\n')
	{
		switch(s[i])
		{
			case '.':
			case '#':
				return;
			case '\t':
			case ' ':
				i++;
				break;
			default:
				int start = i;
				if(isAlphanumeric(s[i]))
				{
					validAlphanumeric(s, &i);
					if(s[i] == ':')
						return;
					// found valid instruction
					// get string from start to i(not inclusive)
						int length = i - start ;
						char* instruction = (char*)malloc(length * sizeof(char));
						//memcpy(instruction, s + start, length);
						for(int j = start; j < i; j++)
						{
							instruction[j - start] = s[j];
						}
						addToTable(&table, instruction, length);
				}

				return;
		}
	}
}


int main()
{
	FILE* fd;
	fd = fopen("input.txt", "r");

	char* buffer = NULL; 
	size_t buf_size = 256;
	ssize_t bytes_read;
	buffer = malloc(buf_size * sizeof(char));
	initTable(&table);

	while((bytes_read = getline(&buffer, &buf_size, fd)) != -1)
	{
		parseLine(buffer);
	}

	int count = 0;
	qsort(table.entries, table.size, sizeof(Entry), cmp);
	for(int i = 0; i < table.size; i++)
	{
		Entry entry = table.entries[i];
		if(entry.key != NULL)
		{
			printf("%d ", count++);
			for(int j = 0; j < entry.keyLength; j++)
				printf("%c", entry.key[j]);
			printf(":%d\n", entry.value);
		}
	}
	char** data = (char**)malloc(count * sizeof(char*));
	int* freq = (int*)malloc(count * sizeof(int));
	count = 0;
	for(int i = 0; i < table.size; i++)
	{
		Entry entry = table.entries[i];
		if(entry.key != NULL)
		{
			data[count] = entry.key;
			freq[count] = entry.value;
			count++;
		}
	}
	int size = sizeof(freq) / sizeof(freq[0]);
	printf("\n");
	HuffmanCodes(data, freq, count);
	freeTable(&table);
	fclose(fd);
	free(buffer);
}
