#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"

static void errorAt(Token token, const char* message)
{
	fprintf(stderr, "[line %d] Error", token->line);
	if (token->type == TOKEN_EOF)
	{
		fprintf(stderr, " at end");
	}
	else if (token->type == TOKEN_ERROR)
	{
	}
	else
	{
		fprintf(stderr, " at '%.*s'", token->length, token->start);
	}

	fprintf(stderr, ": %s\n", message);
	parser.hadError = true;
}

static void errorAtCurrent(Parser* parser, const char* message)
{
	errorAt(parser->current, message);
}

static void error(Parser* parser, const char* message)
{
	errorAt(parser->previous, message);
}

static void advance(Parser* parser, Scanner* scanner)
{
	parser->previous = parser->current;

	for (;;)
	{
		parser->current = scanToken(scanner);
		if(parser->current.type != TOKEN_ERR)
			break;

		errorAtCurrent(parser, );
	}
}

static void consume(Parser* parser, TokenType type, const char* message)
{
	if (parser->current.type == type)
	{
		advance();
		return;
	}

	errorAtCurrent(message);
}

static bool check(Parser* parser, TokenType type)
{
	return parser->current.type == type;
}

static void emitByte(Byte byte)
{
	addByte(byte);
}

static bool isMacro(Parser* parser)
{
}

static bool match(char* instruction, int start, char* pattern, int length)
{
	if(memcmp(instruction + start, pattern, length) == 0)
		return true;

	return false;
}

static void commaStatement(Parser* parser, Scanner* scanner)
{
	int value = expressionStatement(parser, scanner);

	while(check(parser, TOKEN_COMMA))
	{
		advance(parser, scanner);
		value = expressionStatement(parser, scanner);
	}
}

static void instructionStatement(Parser* parser, Scanner* scanner)
{
	advance(parser, scanner);
	
	if(parser->current.type == TOKEN_INSTRUCTION)
	{
		emitValue(getStringFromTable(parser->current.lexeme));	// something along those lines
		commaExpression();
	}
	else
	{
		errorAtCurrent(parser, "Unknown instruction.");
		skipLine();
	}
}

static void labelStatement(Parser* parser, Scanner* scanner)
{
	char* word = (char*)malloc((parser->previous.length + 1) * sizeof(char));
	word = parser->previous.start;
	word[length] = '\0';

	if(isMacro(parser))
	{
		advance(parser, scanner);

	}
	instructionStatement(parser, scanner);
}

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

void parse(Parser* parser, Scanner* scanner)
{
	advance(parser, scanner);

	if(parser.previous == TOKEN_SECTION)
	{
		newSection(parser, scanner);
	}
	else if(parser.previous == TOKEN_ENTRY)
	{
		programEntry(parser, scanner);
	}
	else if(parser.previous == TOKEN_LABEL)
	{
		labelStatement(parser, scanner);
	}
	else
	{	
		instructionStatement(parser, scanner);
	}
}
