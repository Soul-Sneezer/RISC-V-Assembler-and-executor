#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "common.h"
#include "trie.h"

int32_t parseNumber(char* buffer, int32_t* index)
{
	int32_t number = 0;
	while(buffer[*index] != '\0' && buffer[*index] != '\n')
	{
		number = number* 10 + (buffer[*index] - '0');
		(*index)++;
	}

	return number;
}

static bool isDelimiter(char c)
{
	return (c == ' ' || c == ':' || c == '\t' || c == '\r' || c == '\n');
}

static char* getString(char* buffer, int32_t* buf_index, int32_t length)
{
	char* new_string;

	new_string = (char*)malloc((length + 1) * sizeof(char));
	new_string[length] = '\0';
	for(int32_t i = 0; i < length; i++)
	{
		new_string[i] = buffer[*buf_index - length +  i];
	}
	return new_string;
}

char* readFile(const char* path)
{
	FILE* fd = fopen(path, "rb");
	fseek(fd, 0L, SEEK_END);
	size_t file_size = ftell(fd);
	rewind(fd);

	char* buffer = (char*)malloc(file_size + 1);
	size_t bytes_read = fread(buffer, sizeof(char), file_size, fd);
	buffer[bytes_read] = '\0';

	fclose(fd);
	return buffer;
}

void charToLowercase(char* c)
{
    *c = tolower(*c);
}

void strToLowercase(char* s)
{
	for(int32_t i = 0; s[i] != '\0'; i++)
	{
        charToLowercase(&s[i]);
	}
}

static void skipLine(char* buffer, int32_t* index)
{
	for(;;)
	{
		char c = buffer[*index];
		if(c != '\n' && c != '\0')
			*index++;
		else
			return;
	}
}

static void skipWhitespace(char* buffer, int32_t* index)
{
	for(;;)
	{
		char c = buffer[*index];

		switch(c)
		{
			case ':':
			case ' ':
			case '\t':
			case '\r':
				(*index)++;
				break;
			case ';':
				skipLine(buffer, index);
				return;
			default:
				return;
		}
	}
}

void copyWord(char** from, char** to, int32_t length)
{
	for(int32_t i = 0; i < length; i++)
	{
		(*to)[i] = (*from)[i];
	}
}

static void assignInstructionEncoding(TrieNode* node)
{
	char* buffer = readFile("instructions.txt");
	
	int32_t index = 0;
	int32_t n = 0;
	char** words = (char**)malloc(256 * sizeof(char*));
	int32_t* values = (int32_t*)malloc(256 * sizeof(int32_t));

	while(buffer[index] != '\0')
	{
		int32_t start = index;
		while(buffer[index] != ' ')
		{
			index++;
		}

		int32_t length = index - start;
		char* word = (char*)malloc((length + 1) * sizeof(char));

		for(int32_t i = 0; i < length; i++)
		{
			word[i] = buffer[start + i];
		}

		word[length] = '\0';
		toLowercase(&word);
		words[n] = word;
		index++;

		values[n++] = parseNumber(buffer, &index);

		if(buffer[index] == '\n')
			index++;
	}

	for(int32_t i = 0; i < n; i++)
	{
		insertNode(node, words[i], values[i]);
	}

	free(buffer);
}
