#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

void tokenInfo(Token token)
{
	if(token.type == 8)
		return;
	printf("%d ", token.line);
	for (int32_t i = 0; i < token.length; i++)
		printf("%c", token.start[i]);
	printf(" : ");
	int32_t type = token.type;

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

static void printByteAsHex(Byte byte)
{
	uint8_t digit = byte % 16;
	char output[2]
	switch(digit)
	{
		case 10:
			output[1] = 'A';
			break;
		case 11:
			output[1] = 'B';
			break;
		case 12:
			output[1] = 'C';
			break;
		case 13:
			output[1] = 'D';
			break;
		case 14:
			output[1] = 'E';
			break;
		case 15:
			output[1] = 'F';
			break;
		default:
			output[1] = digit + '0';
	}

	byte /= 16;

	switch(byte)
	{
		case 10:
			output[0] = 'A';
			break;
		case 11:
			output[0] = 'B';
			break;
		case 12:
			output[0] = 'C';
			break;
		case 13:
			output[0] = 'D';
			break;
		case 14:
			output[0] = 'E';
			break;
		case 15:
			output[0] = 'F';
			break;
		default:
			output[0] = digit + '0';
	}
}

// these functions should help with debugging the vm
// and any program written in the assembly language defined
// maybe try making a true debugger out of this?
// with this I can finally get to debug a debugger
// seems like a special kind of hell
void dumpHeap(VM* vm)
{
	for(int32_t i = 0; i < heap_size; i++)
	{
		Byte byte = vm->cpu_mem->heap;
		printByteAsHex(byte);
		vm->cpu_mem.heap++;
	}
}

void dumpLabels(VM* vm)
{
}

void dumpCode(VM* vm)
{
	for(int32_t i = 0; i < heap_size; i++)
	{
		Byte byte = vm->cpu_mem->heap;
		printByteAsHex(byte);
		vm->cpu_mem.heap++;
	}
}

void dumpRegisters(VM* vm)
{
	printf("Register values: \n");
	for(int32_t i = 0; i < 64; i++)
	{
		if(i < 32)
			printf("                X");
		else
			printf("                F");

		printf("%d : ", i);
		printf("%d\n", vm->registers[i]);
	}
}

void dumpStack(VM* vm)
{
	while(vm->cpu_mem.base_pointer != vm->cpu_mem.stack_top)
	{
		Byte byte = vm->cpu_mem->base_pointer;
		printByteAsHex(byte);
		vm->cpu_mem.base_pointer--; // the stack starts at the top and goes down
	}
}

void showState(VM* vm)
{
	dumpStack(vm);
	dumpHeap(vm);
	dumpCode(vm);
	dumpRegisters(vm);
}

