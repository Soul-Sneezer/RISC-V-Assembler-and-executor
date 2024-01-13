#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

static bool isAtEnd(VM* vm)
{
	return vm->ip == NULL;
}

static Byte nextByte(VM* vm)
{
	if(!isAtEnd(vm))
		return *(vm->ip++);
	
	return NULL;
}

static Byte getByte(VM* vm)
{
	if(!isAtEnd(vm))
		return *vm->ip;

	return NULL;
}

static Byte getBit(Byte byte, int index)
{
	return byte[index] & 0x1;
}

static Byte getRegister(int* starting_index)
{
	if(*starting_index == 8)
		*starting_index = 0;

	int start = *starting_index;
	int current_bit = 0;
	uint8_t reg_index = 0;
	while(current_bit - start != 6)
	{
		reg_index *= 2;
		reg_index += getBit(instruction, current_bit);
		*starting_index++;
		current_bit++;
		if(*starting_index == 8)
		{
			*starting_index = 0;
			instruction = nextByte();
		}
	}
	return reg_index;
}

static uint32_t getImmediate()
{
}

static void getName()
{
	// search until you find the equivalent of '\0'
}

static Byte* findLocation(char* name)
{
}

static void jumpToLocation(Byte* location)
{
	vm->ip = location;
}

void execute(VM* vm)
{
	if(isAtEnd(vm))
		return;

	Byte instruction = getByte(vm);
	uint8_t index = 0;
	int current_bit = 0;
	while(current_bit < 8 && instruction[index] == -1)
	{
		// find the instruction
		index *= 2
		if (getBit(instruction, current_bit) == 1)
			index += 2;
		else
			index += 1;
		current_bit++;
	}
  uint8_t instruction_index = instruction[index];
	switch(instruction_index)
	{
		case OP_BEQZ: // branch if zero
		{							// expects a register and a position as arguments
			uint8_t reg = getRegister();
			char* name = getName();
			Byte* location = findLocation();
			if (registers[reg] == 0)
				jumpToLocation(location);
		}
			break;
		case OP_FMULS:
			break;
		case OP_FADDD:
			break;
		case OP_BNEZ:
			break;
		case OP_SUB:
			break;
		OP_FMULD:
			break;
		case OP_BGE:
			break;
		case OP_FLD:
			break;
		case OP_FADDS:
			break;
		case OP_FSQRT:
			break;
		case OP_FLW:
			break;
		case OP_BLE:
			break;
		case OP_FLT:
			break;
		case OP_FSUB:
			break;
		case OP_SRAI:
			break;
		case OP_SLLI:
			break;
		case OP_BGT:
			break;
		case OP_FMV:
			break;
		case OP_LD:
			break;
		case OP_LB:
			break;
		case OP_SD:
			break;
		case OP_MV:
			break;
		case OP_LW:
			break;
		case OP_SB:
			break;
		case OP_CALL:
			break;
		case OP_RET:
			break;
		case OP_ADD:
			break;
		case OP_LI:
			break;
		case OP_ADDI:
			break;
		case OP_SFW:
			break;
		case OP_LA:
			break;
		case OP_J:
			break;
		default:
			runtimeError("Unknown instruction.");
	}
}
