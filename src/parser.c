#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

Parser* initParser(char** instructions, char** instruction_values, char** registers, char** register_values, int size)
{
	Parser* parser = (Parser*)malloc(sizeof(Parser));
	initTable(parser->instructions);
	initTable(parser->registers);
	
	for(int i = 0; i < size; i++)
	{
		addStringToTable(parser->instructions, instructions[i], instruction_values[i]);
		addStringToTable(parser->registers, registers[i], register_values[i]);
	}
}
