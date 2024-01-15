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

		int instructions_size;
		int registers_size;
		char** instruction_names;
		char** instruction_values;
		importFile("instructions.txt", &instructions_size, &instruction_names, &instruction_values);
		scanner->instructions = getNode();
		createTrie(scanner->instructions, instruction_names, instructions_size);

		char** register_names = (char**)malloc(256 * sizeof(char*));
		char** register_values;
		importFile("registers.txt", &registers_size, &register_names, &register_values);
		scanner->registers = getNode();
		createTrie(scanner->registers, register_names, registers_size);
		
		Parser* parser = initParser(instruction_names, instruction_values, register_names, register_values, "header.kelp", "code.ok", instructions_size, registers_size);

		parse(parser, scanner);
		freeScanner(scanner);
		freeParser(parser);
	}
	else
	{
		fprintf(stderr, "Usage: risk [path]\n");
		exit(64);
	}

	return 0;
}
