#include <stdio.h>
#include <stdlib.h>
#include "executor.h"
#include "binary_tree.h"
// Here we have the instruction opcodes


int main(int argc, char argv[])
{
	if(argc == 1)
	{
		printf("ERROR: There is no file to execute.");
	}
	readFile();
	readInstructionFile();
	generateInstructions();
	Byte* current_byte = getByte();
	for(int i = 0; i < 8; i++)
	{
		current_byte->bits[i];
	}
}
