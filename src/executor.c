#include <stdio.h>
#include <stdlib.h>
#include "executor.h"
#include "binary_tree.h"

static bool isAtEnd(CPU* cpu)
{
	return cpu->ip == NULL;
}

static Byte nextByte(CPU* cpu)
{
	if(!isAtEnd(cpu))
		return *(cpu->ip++);
	
	return NULL;
}

static Byte getByte(CPU* cpu)
{
	if(!isAtEnd(cpu))
		return *cpu->ip;

	return NULL;
}

void execute(CPU* cpu)
{
	if(isAtEnd(cpu))
		return;

	Byte instruction = getByte(cpu);

	// find the instruction
}
