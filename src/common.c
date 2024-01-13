#include <stdio.h>
#include <stdlib.h>
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

char* getString(char* buffer, int length, int buf_index)
{
	if(length <= 1)
		return NULL;
	
	int i = 0;
	char* new_string = (char*)malloc(length * sizeof(char));
	new_string[length - 1] = '\0';
	for(; (i + 1 - length) != 0; i++)
	{
		new_string[i]	= buffer[buf_index - length + 1 + i];	
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
	for(int i = 0; i < (*s)[i] != '\0'; i++)
	{
		if((*s)[i] >= 'A' && (*s)[i] <= 'Z')
			(*s)[i] += 32;
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
				*index++;
				break;
			case '%':
				skipLine(buffer, index);
				return;
			default:
				return;
		}
	}
}

static bool isDelimiter(char c)
{
	return (c == ' ' || c == ';' || c == '\t' || c == '\r' || c == '\n');
}

void importFile(const char* path, int* size, char** s, char** v)
{
	bool value = false;
	char** strings = (char**)malloc(256 * sizeof(char*));
	char** values = (char**)malloc(256 * sizeof(char*));
	char* buffer = readFile(path);
	uint8_t length = 0;
	uint8_t index = 0;
	int buf_index = 0;

	while(buffer[buf_index] != '\0')
	{
		skipWhitespace(buffer, &buf_index);

		if(isDelimiter(buffer[buf_index])
		{
			char* new_string = getString(buffer, length, buf_index);
			if(new_string != NULL)
			{
				if(!value)
					strings[index] = new_string;
				else
					values[index++] = new_string;
			}
			length = 0;
			value = !value;
			skipWhitespace(buffer, &buf_index);
		}
		}
		else
		{
			buf_index++;
			length++;
		}
	}
	*size = index;
	s = strings;
	v = values;
}
