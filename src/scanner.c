#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "common.h"
#include "trie.h"
//#include "vm.h"

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

static bool isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

static bool isHexadecimal(char c)
{
	toLowercaseC(&c);
	return ((c >= '0'  && c <= '9') || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e');
}

static bool isAlphanumeric(char c)
{
		return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == ':');
}

static bool match(char* word, int length1, char* other_word, int length2)
{
	if(length1 != length2)
		return false;

	return strcmp(word, other_word);
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

	if(findWord(scanner->instructions, word))
		return TOKEN_INSTRUCTION;

	// check if it's a register
	if(findWord(scanner->registers, word))
		return TOKEN_REGISTER;

	bool immediate = true;
	for(int i = 0; i < length; i++)
	{
		if(!isDigit(word[i]))
			immediate = false;
	}
	if(immediate)
		return TOKEN_IMMEDIATE;
	return TOKEN_LABEL; // otherwise it's a label of some kind
}

Scanner* initScanner(char* source)
{
	Scanner* scanner = (Scanner*)malloc(sizeof(Scanner));
	scanner->start = source;
	scanner->current = source;
	scanner->line = 1;

	return scanner;
}

static Token identifier(Scanner* scanner)
{
	while(isAlphanumeric(peek(scanner)))
	{
		advance(scanner);
	}

	return makeToken(scanner, identifierType(scanner));
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

	if(isAtEnd(scanner))
		return makeToken(scanner, TOKEN_EOF);
	
	char c = advance(scanner);

	switch(c)
	{
		case ',': return makeToken(scanner, TOKEN_COMMA);
		case '(': return makeToken(scanner, TOKEN_LEFT_PAREN);
		case ')': return makeToken(scanner, TOKEN_RIGHT_PAREN);
		case '-': return makeToken(scanner, TOKEN_MINUS);
		case '.':
		{
			bool isSection = true;
			bool skipKeyword = false;
			while(peek(scanner) != '\n')
			{
				advance(scanner);
				if(peek(scanner) == ' ' || peek(scanner) == '\t' || peek(scanner) == '\r')
				{
					Token token = makeToken(scanner, TOKEN_SECTION);
					if(match(token.start, token.length, ".global", 7))
							isSection = false;
					skipKeyword = true;
					skipWhitespace(scanner);
					scanner->start = scanner->current;
				}
			}
			Token token = makeToken(scanner, TOKEN_SECTION);
			if(!isSection)
				token.type = TOKEN_ENTRY;
			advance(scanner);
			return token;
		}
		
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
		default:
		{
			if(isAlphanumeric(c))
			{
				return identifier(scanner);
			}
		return errorToken(scanner, "Unexpected character.");
		}
	}
}

void freeScanner(Scanner* scanner)
{

}
/* this is for testing
int main(int argc, char* argv[])
{
	char* source = readFile(argv[1]);
	Scanner* scanner = initScanner(source);
	int size = 0;
	char** words = (char**)malloc(256 * sizeof(char*));
	char** values = (char**)malloc(256 * sizeof(char*));
	words = importFile("instructions.txt", &size, words, values);
	scanner->instructions = getNode();
	createTrie(scanner->instructions, words, size);

	words = importFile("registers.txt", &size, words, values);
	scanner->registers = getNode();

	createTrie(scanner->registers, words, size);

	Token token;
	//token = scanToken(scanner);
	//token = scanToken(scanner);
	while((token = scanToken(scanner)).type != TOKEN_EOF)
	{
		for(int i = 0; i < token.length; i++)
			printf("%c", token.start[i]);
		printf(" ");
	}
}
*/
