#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "parser.h"
#include "vm.h"
#include "common.h"

typedef enum
{
	SUCCESS,
	COMPILE_ERROR,
	RUNTIME_ERROR,
} ErrorType;

ErrorType runFile(const char* source)
{
}

int main(int argc, const char* argv[])
{
	if (argc == 2)
	{
		const char* source = readFile(argv[1]);
		Scanner* scanner = initScanner(source);
		Parser* parser = initParser();

		int size;
		char** words = (char**)malloc(256 * sizeof(char*));
		char** values;
		importFile("instructions.txt", &size, words, &values);
		createInstructionTable(parser, size, words, values);
		scanner->instructions = getNode();
		for(int i = 0; i < size; i++)
		{
			toLowercase(&words[i]);
		}
		createTrie(scanner->instructions, words, size);
	
		importFile("registers.txt", &size, words, values);
		createRegisterTable(parser, size, words, values);
		scanner->registers = getNode();
		for(int i = 0; i < size; i++)
		{
			toLowercase(&words[i]);
		}
		createTrie(scanner->registers, words, size);
		
		parse(scanner, parser);
	
		freeScanner(scanner);
	}
	else
	{
		fprintf(stderr, "Usage: risk [path]\n");
		exit(64);
	}

	return 0;
}
