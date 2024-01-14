#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"

static void skipLine(Parser* parser, Scanner* scanner)
{
	int line = parser->current.line;
	while(parser->current.line == line)
		advance(parser, scanner);
}

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
	skipLine();
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

static uint8_t invertByte(uint8_t byte)
{
	uint8_t inverse_byte = 0;
	for(int i = 0; i < 8; i++)
	{
		inverse_byte <<= 1;
		inverse_byte += byte & 0x1;
		byte >>= 1;
	}

	return inverse_byte;
}

static void writeByte(uint8_t byte, FILE* fd)
{
	byte = invertByte();
	fwrite(fd, "%d ", byte);
}

static void writeByteAsChar(uint8_t byte, FILE* fd)
{
	byte = invertByte(byte);	

	for(int i = 0; i < 8; i++)
	{
		if(byte & 0x1)
			fprintf(fd, "1");
		else
			fprintf(fd, "0");
		byte >>= 1;
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

static bool match(char* instruction, int start, char* pattern, int length)
{
	if(memcmp(instruction + start, pattern, length) == 0)
		return true;

	return false;
}

static void memoryAccess(Parser* parser, Scanner* scanner)
{
	if(parser->current.type == TOKEN_REGISTER)
	{
		reg(parser);
		advance(parser, scanner);
		consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after memory adress.");
	}
	else
	{
		errorAtCurrent("Invalid memory access.");
	}
}

static uint32_t immediate(Parser* parser)
{
	int value = 0;
	for(int i = 0; i < parser->previous.length; i++)
	{
		value = value * 10 + (parser->previous.start[i] - '0');
	}
	writeByte(value >> 16 & 0xFF, parser->code_fd);
	writeByte(value >> 8 & 0xFF, parser->code_fd);
	writeByte(value & 0xFF, parser->code_fd);

	writeByteAsChar(value >> 16 & 0xFF, parser->tcode_fd);
	writeByteAsChar(value >> 8 & 0xFF, parser->tcode_fd);
	writeByteAsChar(value & 0xFF, parser->tcode_fd);
}

static uint32_t reg(Parser* parser)
{
	int index;
	if(findInTable(parser->registers), parser->previous, &index)
	{
		writeByte(index, parser->code_fd);
		writeByteAsChar(index, parser->tcode_fd);
	}
}

static void expressionStatement()
{
	advance(parser, scanner);
	// it's either a register, or an immediate, or memory
	if(check(parser, TOKEN_LEFT_PAREN))
	{
		advance();
		memoryAccess();
	}
	else if(check(parser, TOKEN_IMMEDIATE)) // there must be a minus before
	{
		immediate();
	}
	else if(parser->previous.type == TOKEN_IMMEDIATE)
	{
		immediate();	
	}
	else if(parser->previous.type == TOKEN_REGISTER)
	{
		reg();
	}
	else
	{
		error(parser, "Wrong operand types.");
	}
}

static void commaStatement(Parser* parser, Scanner* scanner)
{
	int value = expressionStatement(parser, scanner);

	while(check(parser, TOKEN_COMMA))
	{
		advance(parser, scanner);
		expressionStatement(parser, scanner);
	}
}

static void instructionStatement(Parser* parser, Scanner* scanner)
{
	advance(parser, scanner);
	
	if(check(parser, TOKEN_INSTRUCTION))
	{
		char* lexeme = (char*)malloc((parser->current.length + 1) * sizeof(char));
		lexeme[length] = '\0';
		emitValue(getStringFromTable(lexeme));	// something along those lines
		commaExpression(parser, scanner);
	}
	else
	{
		errorAtCurrent(parser, "Unknown instruction.");
	}
}

static void labelStatement(Parser* parser, Scanner* scanner)
{
	char* word = (char*)malloc((parser->previous.length + 1) * sizeof(char));
	word = parser->previous.start;
	word[length] = '\0';

	// add label to header

	instructionStatement(parser, scanner);
}

Parser* initParser(char** instructions, char** instruction_values, char** registers, char** register_values, const char* header_file, const char* code_file, int size)
{
	Parser* parser = (Parser*)malloc(sizeof(Parser));
	initTable(parser->instructions);
	initTable(parser->registers);
	
	for(int i = 0; i < size; i++)
	{
		addStringToTable(parser->instructions, instructions[i], instruction_values[i]);
		addStringToTable(parser->registers, registers[i], register_values[i]);
	}

	parser->code_fd = openCodeFile("code.ok");
	parser->header_fd = openHeaderFile("header.kelp");

	parser->tcode_fd = openCodeTestFile("code_not.ok");
	parser->theader_fd = openHeaderTestFile("header_send.kelp");
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
