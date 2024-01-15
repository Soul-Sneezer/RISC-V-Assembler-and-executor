#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

void token_info(Token token)
{
	if(token.type == 8)
		return;
	printf("%d ", token.line);
	for (int i = 0; i < token.length; i++)
		printf("%c", token.start[i]);
	printf(" : ");
	int type = token.type;

	switch(type)
	{
		case 0:
			printf("TOKEN_INSTRUCTION\n");
			break;
		case 1:
			printf("TOKEN_REGISTER\n");
			break;
		case 2:
			printf("TOKEN_IMMEDIATE\n");
			break;
		case 3:
			printf("TOKEN_LABEL\n");
			break;
		case 4:
			printf("TOKEN_STRING\n");
			break;
		case 5:
			printf("TOKEN_COMMA\n");
			break;
		case 6:
			printf("TOKEN_CONSTANT\n");
			break;
		case 7:
			printf("TOKEN_ERR\n");
			break;
		case 8:
			printf("TOKEN_EOF\n");
			break;
		case 9:
			printf("TOKEN_SECTION\n");
			break;
		case 10:
			printf("TOKEN_ENTRY\n");
			break;
		case 11:
			printf("TOKEN_LEFT_PAREN\n");
			break;
		case 12:
			printf("TOKEN_RIGHT_PAREN\n");
			break;
		case 13:
			printf("TOKEN_MINUS\n");
			break;
		case 14:
			printf("TOKEN_SPACE\n");
			break;
		case 15:
			printf("TOKEN_ASCIZ\n");
			break;
	}
}
