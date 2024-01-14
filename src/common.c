#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "common.h"

int parseNumber(char* buffer, int* index)
{
	int number = 0;
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

static char* getString(char* buffer, int* buf_index, int length)
{
	char* new_string;

	new_string = (char*)malloc((length + 1) * sizeof(char));
	new_string[length] = '\0';
	for(int i = 0; i < length; i++)
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

void toLowercaseC(char* c)
{
	if(*c >= 'A' && *c <= 'Z')
		(*c) += 32;
}

void toLowercase(char** s)
{
	for(int i = 0; (*s)[i] != '\0'; i++)
	{
		if((*s)[i] >= 'A' && (*s)[i] <= 'Z')
			(*s)[i] += 32;
	}
}

static void skipLine(char* buffer, int* index)
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

static void skipWhitespace(char* buffer, int* index)
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

char** importFile(const char* path, int* size, char** s, char*** values) // I'm not proud of this
{
	bool value = false;
	char** strings = (char**)malloc(256 * sizeof(char*));
	*values = (char**)malloc(256 * sizeof(char*));
	char* buffer = readFile(path);
	int index = 0;
	int buf_index = 0;
	int length = 0;

	skipWhitespace(buffer, &buf_index);

	while(buffer[buf_index] != '\0')
	{
		if(isDelimiter(buffer[buf_index]))
		{
			char* new_string = getString(buffer, &buf_index, length);
			if(new_string != NULL)
			{
				if(!value)
					strings[index] = new_string;
				else
					(*values)[index++] = new_string;
			}
			value = !value;
			length = 0;
			buf_index++;
			skipWhitespace(buffer, &buf_index);
		}
		else if(buffer[buf_index] != '\0')
		{
			buf_index++;
			length++;
		}
	}
	*size = index;
	return strings;
}
