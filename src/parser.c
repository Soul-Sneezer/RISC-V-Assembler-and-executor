#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"

static void skipLine(Parser* parser, Scanner* scanner);

static void errorAt(Parser* parser, Scanner* scanner, Token token, const char* message)
{
	fprintf(stderr, "[line %d] Error", token.line);
	if (token.type == TOKEN_EOF)
	{
		fprintf(stderr, " at end");
	}
	else if (token.type == TOKEN_ERR)
	{
	}
	else
	{
		fprintf(stderr, " at '%.*s'", token.length, token.start);
	}

	fprintf(stderr, ": %s\n", message);
	skipLine(parser, scanner);
}

static void errorAtCurrent(Parser* parser, Scanner* scanner, const char* message)
{
	errorAt(parser, scanner, parser->current, message);
}

static void error(Parser* parser, Scanner* scanner, const char* message)
{
	errorAt(parser, scanner, parser->previous, message);
}

static void advance(Parser* parser, Scanner* scanner)
{
	parser->previous = parser->current;

	for (;;)
	{
		parser->current = scanToken(scanner);
		if(parser->current.type != TOKEN_ERR)
			break;

		errorAtCurrent(parser, scanner, "Unknown keyword.");
	}
}

static void skipLine(Parser* parser, Scanner* scanner)
{
	int line = parser->current.line;
	while(parser->current.line == line)
		advance(parser, scanner);
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
	byte = invertByte(byte);
	fwrite(&byte, 1, 1, fd);
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

static void consume(Parser* parser, Scanner* scanner, TokenType type, const char* message)
{
	if (parser->current.type == type)
	{
		advance(parser, scanner);
		return;
	}

	errorAtCurrent(parser, scanner, message);
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

static uint32_t reg(Parser* parser, Scanner* scanner)
{
	int index;
	char* lexeme = (char*)malloc((parser->previous.length + 1) * sizeof(char));
	lexeme = parser->previous.start;
	lexeme[parser->previous.length] = '\0';
	if(getValueFromTable(parser->registers, lexeme) != -1)
	{
		writeByte(index, parser->code_fd);
		writeByteAsChar(index, parser->tcode_fd);
	}
	else
	{
		error(parser, scanner, "Invalid register.");
	}
}

static void memoryAccess(Parser* parser, Scanner* scanner)
{
	if(parser->current.type == TOKEN_REGISTER)
	{
		reg(parser, scanner);
		advance(parser, scanner);
		consume(parser, scanner, TOKEN_RIGHT_PAREN, "Expected ')' after memory adress.");
	}
	else
	{
		errorAtCurrent(parser, scanner, "Invalid memory access.");
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

static void expressionStatement(Parser* parser, Scanner* scanner)
{
	advance(parser, scanner);
	// it's either a register, or an immediate, or memory
	if(check(parser, TOKEN_LEFT_PAREN))
	{
		advance(parser, scanner);
		memoryAccess(parser, scanner);
	}
	else if(check(parser, TOKEN_IMMEDIATE)) // there must be a minus before
	{
		immediate(parser);
	}
	else if(parser->previous.type == TOKEN_IMMEDIATE)
	{
		immediate(parser);	
	}
	else if(parser->previous.type == TOKEN_REGISTER)
	{
		reg(parser, scanner);
	}
	else
	{
		error(parser, scanner, "Wrong operand types.");
	}
}

static void commaStatement(Parser* parser, Scanner* scanner)
{
	expressionStatement(parser, scanner);

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
		lexeme[parser->current.length] = '\0';
		int instr;
		if((instr = getValueFromTable(parser->instructions, lexeme)) != -1)
		{
			writeByte(instr, parser->code_fd);
			writeByteAsChar(instr, parser->tcode_fd);
		}
		else
		{
			errorAtCurrent(parser, scanner, "Invalid instruction.");
		}
		commaStatement(parser, scanner);
	}
	else
	{
		errorAtCurrent(parser, scanner, "Unknown instruction.");
	}
}

static void labelStatement(Parser* parser, Scanner* scanner)
{
	char* word = (char*)malloc((parser->previous.length + 1) * sizeof(char));
	word = parser->previous.start;
	word[parser->current.length] = '\0';

	// add label to header

	instructionStatement(parser, scanner);
}

static void newSection(Parser* parser, Scanner* scanner)
{
}

static void programEntry(Parser* parser, Scanner* scanner)
{
}

static void createInstructionTable(Parser* parser, int size, char** instructions, char** values)
{
	for(int i = 0; i < size; i++)
	{
		addStringToTable(parser->instructions, instructions[i], values[i]);
	}
}

static void createRegisterTable(Parser* parser, int size, char** registers, char** values)
{
	for(int i = 0; i < size; i++)
	{
		int j = 0;
		int value = 0;
		while(values[i][j] != '\0')
		{
			value = value * 10 + (values[j++] - '0');
		}
		addValueToTable(parser->registers, registers[i], value); 
	}
}

void openCodeFile(Parser* parser, const char* code_file)
{
	FILE* fd = fopen(code_file, "wb");
	parser->code_fd = fd;
}

void openHeaderFile(Parser* parser, const char* header_file)
{
	FILE* fd = fopen(header_file, "wb");
	parser->header_fd = fd;
}

void openCodeTestFile(Parser* parser, const char* code_file)
{
	FILE* fd = fopen(code_file, "w");
	parser->tcode_fd = fd;
}

void openHeaderTestFile(Parser* parser, const char* header_file)
{
	FILE* fd = fopen(header_file, "wb");
	parser->theader_fd = fd;
}

Parser* initParser(char** instructions, char** instruction_values, char** registers, char** register_values, const char* header_file, const char* code_file, int size)
{
	Parser* parser = (Parser*)malloc(sizeof(Parser));

	createInstructionTable(parser, size, instructions, instruction_values);
	createRegisterTable(parser, size, registers, register_values);

	initTable(parser->instructions);
	initTable(parser->registers);
	
	for(int i = 0; i < size; i++)
	{
		addStringToTable(parser->instructions, instructions[i], instruction_values[i]);
		addStringToTable(parser->registers, registers[i], register_values[i]);
	}

	openCodeFile(parser, code_file);
	openHeaderFile(parser, header_file);

	openCodeTestFile(parser, "code_not.ok");
	openHeaderTestFile(parser, "header_send.kelp");
}

void freeParser(Parser* parser)
{
}

void parse(Parser* parser, Scanner* scanner)
{
	advance(parser, scanner);

	if(parser->previous.type == TOKEN_SECTION)
	{
		newSection(parser, scanner);
	}
	else if(parser->previous.type == TOKEN_ENTRY)
	{
		programEntry(parser, scanner);
	}
	else if(parser->previous.type == TOKEN_LABEL)
	{
		labelStatement(parser, scanner);
	}
	else
	{	
		instructionStatement(parser, scanner);
	}
}
