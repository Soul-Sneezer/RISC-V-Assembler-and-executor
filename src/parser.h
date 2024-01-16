#ifndef RISKV_PARSER
#define RISKV_PARSER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "table.h"
#include "scanner.h"
#include "vm.h"

typedef struct
{
	uint8_t current;
	uint8_t byte;
} Buffer; // when it's full or you've reached EOF, flush to file

typedef struct
{
	Token current;
	Token previous;

	FILE* code_fd;
	FILE* header_fd;

	FILE* tcode_fd;
	FILE* theader_fd;
	Table* instructions;	// used to convert from the token to the proper bytecode
	Table* registers;
	int header_entries;

	int current_byte;
	Buffer* buffer;
} Parser;

Parser* initParser(char** instructions, char** instruction_values, char** registers, char** register_values, const char* header_file, const char* code_file, int i_size, int r_size);
void freeParser(Parser* parser);

void parse(Parser* parser, Scanner* scanner);
#endif
