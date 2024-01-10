#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "common.h"
#include "trie.h"

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

static bool isAtEnd(Scanner* scanner)
{
	return *scanner->current == '\0';
}

static Token makeToken(Scanner* scanner, TokenType type)
{
	Token token;
	token.start = scanner->start;
	token.line = scanner->line;
	token.length = (int)(scanner->current - scanner->start);
	token.type = type;

	return token;
}

static Token errorToken(Scanner* scanner, char* message)
{
	Token token;
	token.start = message;
	token.line = scanner->line;
	token.length = strlen(message);
	token.type = TOKEN_ERR;

	return token;
}

static char advance(Scanner* scanner)
{
	scanner->current++;
	return scanner->current[-1];
}

static char peek(Scanner* scanner)
{
	return *scanner->current;
}

static char peekNext(Scanner* scanner)
{
	if(*scanner->current == '\0') return '\0';

	return scanner->current[1];
}

static void skipWhitespace(Scanner* scanner)
{
	for(;;)
	{
		char c = peek(scanner);

		switch(c)
		{
			case ' ':
			case '\t':
			case '\r':
				advance(scanner);
				break;
			case '\n':
				scanner->line++;
				advance(scanner);
				break;
			case '#': 
			{
				while(!isAtEnd(scanner) && peek(scanner) != '\n')
				{
					advance(scanner);
				}
			break;
			}
			default:
				return;
		}
	}
}

static bool isNumeric(Scanner* scanner)
{
	char c = peek(scanner);
	return (c >= '0' && c <= '9');
}

static bool isHexadecimal(Scanner* scanner)
{
	char c = peek(scanner);
	toLowercaseC(&c);
	return ((c >= '0'  && c <= '9') || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e');
}

static bool isAlphanumeric(Scanner* scanner)
{
		char c = peek(scanner);
		return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

static bool match(char* word, int length, char* other_word, int length2)
{
	if(length1 != length2)
		return false;

	return strcmp(word, other_word);
}

static bool isRegister(char* word, int length)
{
	if(length > 3 || length < 2)
		return false;
	
	if(word[0] == 'x' || word[0] == 'f')
	{
		if(length == 2 && isNumeric(word[1]))
			return true;
		else if(length == 3 && (word[1] == '1' || word[1] == '2') && isNumeric(word[2]))
			return true;
		else if(length == 3 && word[1] == '3' && (word[2] == '0' || word[2] == '1' || word[2] == '2'))
			return true;
	}

	return false;
}

static TokenType identifierType(Scanner* scanner)
{
	int length = scanner->current - scanner->start;
	char* word = (char*)malloc((length + 1) * sizeof(char));
	word[length] = '\0';
	for(int i = 0; i < length; i++)
	{
		word[i] = *(scanner->start + i);
	}
	toLowercase(&word);

	if(findWord(scanner->trie, word))
		return TOKEN_INSTRUCTION;

	// check if it's a registers
	if(isRegister(word, length))
		return TOKEN_REGISTER;

	return TOKEN_LABEL; // otherwise it's a label of some kind
}

Scanner* initScanner(char* source)
{
	Scanner* scanner = (Scanner*)malloc(sizeof(Scanner));
	scanner->start = source;
	scanner->current = source;
	scanner->line = 1;

	int size;
	char** words = importInstructions("instructions.txt", &size);
	scanner->trie = getNode();
	for(int i = 0; i < size; i++)
	{
		toLowercase(&words[i]);
	}
	createTrie(scanner->trie, words, size);

	return scanner;
}

static Token immediate(Scanner* scanner)
{
	while(isNumeric(scanner))
	{
		advance(scanner);
	}

	return makeToken(scanner, TOKEN_IMMEDIATE);
}

static Token identifier(Scanner* scanner)
{
	while(isAlphanumeric(scanner))
	{
		advance(scanner);
	}

	return makeToken(scanner, identifierType(scanner));
}

static Token generalRegister(Scanner* scanner)
{
	advance(scanner);
	while(isAlphanumeric(scanner))
	{
		advance(scanner);
	}

	return makeToken(scanner, TOKEN_GENERAL_REGISTER);
}

static Token specialRegister(Scanner* scanner)
{
	advance(scanner);
	while(isAlphanumeric(scanner))
	{
		advance(scanner);
	}

	return makeToken(scanner, TOKEN_SPECIAL_REGISTER);
}

static Token constant(Scanner* scanner)
{
	advance(scanner);
	while(isHexadecimal(scanner))
	{
		advance(scanner);
	}

	return makeToken(scanner, TOKEN_CONSTANT);
}

static Token skipToEndOfLine(Scanner* scanner)
{
	advance(scanner);
	while(peek(scanner) != '\0' && peek(scanner) != '\n')
		advance(scanner);
	return scanToken(scanner);
}

Token scanToken(Scanner* scanner)
{
	skipWhitespace(scanner);
	scanner->start = scanner->current;

	if(isNumeric(scanner))
	{
		return immediate(scanner);
	}
	else if(isAlphanumeric(scanner))
	{
		return identifier(scanner);
	}

	if (isAtEnd(scanner))
		return makeToken(scanner, TOKEN_EOF);

	char c = advance(scanner);

	switch(c)
	{
		case ',': return makeToken(scanner, TOKEN_COMMA);
		case '"':
		{
			advance(scanner);
			while(peek(scanner) != '"' && !isAtEnd(scanner))
			{
				if(isAtEnd(scanner) || peek(scanner) == '\n')
					return errorToken(scanner, "Unterminated string.");
				advance(scanner);
			}

			advance(scanner);

			Token token;
			token.start = scanner->start + 1;
			token.length = scanner->current - scanner->start - 2;
			token.type = TOKEN_STRING;
			token.line = scanner->line;

			return token;
		}
	}
	return errorToken(scanner, "Unexpected character.");
}

void freeScanner(Scanner* scanner)
{

}
