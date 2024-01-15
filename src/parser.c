#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"
#include "debug.h"
#include "common.h"

static void skipLine(Parser* parser, Scanner* scanner);

static void errorAt(Parser* parser, Scanner* scanner, Token token, const char* message)
{
	fprintf(stderr, "[line %d] Error", token.line);
	//if (token.type == TOKEN_EOF)
	//{
	//	fprintf(stderr, " at end");
	//}
	if (token.type == TOKEN_ERR)
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
		token_info(parser->current);
		if(parser->current.type == TOKEN_EOF)
			return;
		if(parser->current.type != TOKEN_ERR)
			break;

		errorAtCurrent(parser, scanner, "Unknown keyword.");
	}
}

static void skipLine(Parser* parser, Scanner* scanner)
{
	int line = parser->current.line;
	while(parser->current.type != TOKEN_EOF && parser->current.line == line)
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

static void writeWord(char* word, FILE* fd)
{
	int n = strlen(word);
	
	uint8_t byte = 34; 
	fwrite(&byte, 1, 1, fd); // add quote mark so it's easier to differentiate
											// between names and values
	for(int i = 0; i < n; i++)
	{
		fwrite(&word[i], 1, 1, fd);
	}
	byte = 3;
	fwrite(&byte, 1, 1, fd); // mark end of string
	byte = 34;
	fwrite(&byte, 1, 1, fd); // closing quote mark
}

static void writeByteAsChars(uint8_t byte, FILE* fd)
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

	fprintf(fd, "\n");
}

static void writeChars(char* string, FILE* fd, FILE* byte_fd)
{
	int n = strlen(string);

	uint8_t byte = 0;
	char output[8];
	for(int i = 0; i < 8; i++)
	{
		output[i] = '0';
	}

	for(int i = 0; i < n; i++)
	{
		output[i] = string[i];
		byte <<= 1;
		if(output[i] == '1')
			byte += 1;
	}
	
	writeByte(byte, byte_fd);
	fprintf(fd, "%s", output);
	fprintf(fd, "\n");
}

static void addToHeader(Parser* parser)
{
	char* word = (char*)malloc((parser->current.length + 1) * sizeof(char));
	copyWord(&parser->current.start, &word, parser->current.length);
	word[parser->current.length] = '\0';

	writeWord(word, parser->header_fd);
	parser->header_entries++;
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
	char* lexeme = (char*)malloc((parser->current.length + 1) * sizeof(char));
	copyWord(&parser->current.start, &lexeme, parser->current.length);
	lexeme[parser->current.length] = '\0';
	int regist;
	if((regist = getValueFromTable(parser->registers, lexeme)) != -1)
	{
		writeByte(regist, parser->code_fd);
		writeByteAsChars(regist, parser->tcode_fd);
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

static void immediate(Parser* parser)
{
	uint32_t value = 0;
	for(int i = 0; i < parser->current.length; i++)
	{
		value = value * 10 + (parser->current.start[i] - '0');
	}
	writeByte(value >> 16 & 0xFFFFFFFF, parser->code_fd);
	writeByte(value >> 8 & 0xFFFFFFFF, parser->code_fd);
	writeByte(value & 0xFFFFFFFF, parser->code_fd);

	writeByteAsChars(value >> 16 & 0xFF, parser->tcode_fd);
	writeByteAsChars(value >> 8 & 0xFF, parser->tcode_fd);
	writeByteAsChars(value & 0xFF, parser->tcode_fd);
}

static void expressionStatement(Parser* parser, Scanner* scanner)
{
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
	else if(check(parser, TOKEN_MINUS))
	{
		immediate(parser);	
	}
	else if(check(parser, TOKEN_REGISTER))
	{
		reg(parser, scanner);
	}
	else if(check(parser, TOKEN_LABEL))
	{
		// then it's either an error(that will be detected at runtime) or control flow
		writeByte(parser->header_entries ,parser->code_fd);
		addToHeader(parser);

	}
	else if(check(parser, TOKEN_EOF))
	{
		// either a ret instruction or an error
		return;
	}
	else
	{
		//token_info(parser->current);
		errorAtCurrent(parser, scanner, "Wrong operand types.");
	}

	advance(parser, scanner);
	if(check(parser, TOKEN_LEFT_PAREN))
	{
		advance(parser, scanner);
		memoryAccess(parser, scanner);
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
	char* lexeme = (char*)malloc((parser->current.length + 1) * sizeof(char));
	lexeme[parser->current.length] = '\0';

	copyWord(&parser->current.start, &lexeme, parser->current.length);

	if(check(parser, TOKEN_INSTRUCTION))
	{
		

		char* instr;
		if((instr = getStringFromTable(parser->instructions, lexeme)) != NULL)
		{
			//writeByte(instr, parser->code_fd);
			//writeByteAsChars(instr, parser->tcode_fd);
			writeChars(instr, parser->tcode_fd, parser->code_fd);
		}
		else
		{
			errorAtCurrent(parser, scanner, "Invalid instruction.");
		}
		advance(parser, scanner);
		commaStatement(parser, scanner);
	}
	else
	{
		errorAtCurrent(parser, scanner, "Unknown instruction.");
	}
}

static void labelStatement(Parser* parser, Scanner* scanner)
{
	addToHeader(parser);
	advance(parser, scanner);
	instructionStatement(parser, scanner);
}

static void newSection(Parser* parser, Scanner* scanner)
{
	addToHeader(parser);
	advance(parser, scanner);
}

static void programEntry(Parser* parser, Scanner* scanner)
{
	addToHeader(parser);
	advance(parser, scanner);
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
			value = value * 10 + (values[i][j++] - '0');
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

Parser* initParser(char** instructions, char** instruction_values, char** registers, char** register_values, const char* header_file, const char* code_file, int i_size, int r_size)
{
	Parser* parser = (Parser*)malloc(sizeof(Parser));

	initTable(&parser->instructions);
	initTable(&parser->registers);

	createInstructionTable(parser, i_size, instructions, instruction_values);
	createRegisterTable(parser, r_size, registers, register_values);

	openCodeFile(parser, code_file);
	openHeaderFile(parser, header_file);

	openCodeTestFile(parser, "code_not.ok");
	openHeaderTestFile(parser, "header_send.kelp");

	parser->header_entries = 0;
}

void freeParser(Parser* parser)
{
}

void parse(Parser* parser, Scanner* scanner)
{
	advance(parser, scanner);

	while(!check(parser, TOKEN_EOF))
	{
			
		if(check(parser, TOKEN_SECTION))
		{
			newSection(parser, scanner);
		}
		else if(check(parser, TOKEN_ENTRY))
		{
			programEntry(parser, scanner);
		}
		else if(check(parser, TOKEN_LABEL))
		{
			labelStatement(parser, scanner);
		}
		else if(check(parser, TOKEN_INSTRUCTION))
		{	
			instructionStatement(parser, scanner);
		}
		else
		{
			error(parser, scanner, "Unknown keyword.");
		}
	}
}
