#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

static Byte getBit(Byte byte, int32_t index)
{
	return byte[index] & 0x1;
}

static Byte getRegister(int32_t* starting_index)
{
	if(*starting_index == 8)
		*starting_index = 0;

	int32_t start = *starting_index;
	int32_t current_bit = 0;
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

static int32_t valueInRegister(uint8_t index);
static void setRegister(uint8_t index);

static uint32_t unsig(int32_t value);

void execute(VM* vm)
{
	if(isAtEnd(vm))
		return;

	Byte instruction = getByte(vm);
	uint8_t index = 0;
	int32_t current_bit = 0;
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
			uint32_t imm = getImmediate();
			if (intFromRegister(reg) == 0)
				jumpToLocation(location);
		}
			break;
		case OP_FMULS:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();
						
			setRegister(rd) = valueInRegister(rs1) * valueInRegister(rs2);
		}
			break;
		case OP_FADDD:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();
					
			setRegister(rd) = valueInRegister(rs1) + valueInRegister(rs2);

		}
			break;
		case OP_BNEZ:
		{
			uint8_t reg = getRegister();
			uint32_t imm = getImmediate();
			if (valueInRegister(reg) != 0)
				jumpToLocation(location);
		}
			break;
		case OP_SUB:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();
						
			setRegister(rd) = valueInRegister(rs1) + valueInRegister(rs2);
		}
			break;
		case OP_FMULD:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();
						
			setRegister(rd) = valueInRegister(rs1) * valueInRegister(rs2);
		}
			break;
		case OP_BGE:
		{
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();
			uint32_t imm = getImmediate();
			

			if(rs2 >= rs1)
				jumpToLocation(location);
		}
			break;
		case OP_FLD:	// load float from memory into register
		{
			uint8_t rd = getRegister();
			double f = getDoubleFromMemory();
			setReigster(rd) = f;
		}
			break;
		case OP_FADDS:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();

			setRegister(rd) = valueInRegister(rs1) + valueInRegister(rs2);
		}
			break;
		case OP_FSQRT:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			double f = valueInRegister(rs1);
			setRegister(rd) = sqrt(f);
		}
			break;
		case OP_FLW:
		{
			uint8_t rd = getRegister();
			float f = getFloatFromMemory();
			setRegister(rd) = f;
		}
			break;
		case OP_BLE:
		{
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();
			uint32_t imm = getImmediate();

			if(valueInRegister(rs1) <= valueInRegister(rs2))
				jumpToLocation(location);
		}
			break;
		case OP_FLT:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();

			if(valueInRegister(rs1) < valueInRegister(rs2))
			{
				setRegister(rd) = 1;
			}
		}
			break;
		case OP_FSUB:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();
			
			setRegister(rd) = valueInRegister(rs1) - valueInRegister(rs2);
		}
			break;
		case OP_SRAI:
		{
			uint8_t rd = getRegister();
			uint32_t imm = getImmediate();

			setRegister(rd) = valueInRegister(rd) >> imm;
		}
			break;
		case OP_SLLI:
		{
			uint8_t rd = getRegister();
			uint32_t imm = getImmediate();

			setRegister(rd) = valueInRegister(rd) << imm;
		}
			break;
		case OP_BGT:
		{
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();
			uint32_t imm = getImmediate();

			if(valueInRegister(rs1) > valueInRegister(rs2))
				jumpToLocation(imm);

		}
			break;
		case OP_FMV:
		{
			uint8_t rd = getRegister();
			uint8_t rs = getRegister();

			setRegister(rd) = valueInRegister(rs);
		}
			break;
		case OP_LD:
		{
			uint8_t rd = getRegister();	// to fix
			setRegister(rd) = getValueFromMemory();
		}
			break;
		case OP_LB:
		{
			uint8_t rd = getRegister();	// to fix
			setRegister(rd) = getByteFromMemory();

		}
			break;
		case OP_SD:
			break;
		case OP_MV:
		{
			uint8_t rd = getRegister();
			uint8_t rs = getRegister();

			setRegister(rd) = valueInRegister(rs);
		}
			break;
		case OP_LW:
		{
			uint8_t rd = getRegister();	// to fix
			setRegister(rd) = getValueFromMemory();
		}
			break;
		case OP_SB:
			break;
		case OP_CALL:
		{
			// save current address using the stack?
			// jump to procedure
		}
			break;
		case OP_RET:
		{
			// go back to original address
		}
			break;
		case OP_ADD:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();

			setRegister(rd) = valueInRegister(rs1) + valueInRegister(rs2);
		}
			break;
		case OP_LI:
			break;
		case OP_ADDI:
		{
			uint8_t rd = getRegister();
			uint8_t rs1 = getRegister();
			uint8_t rs2 = getRegister();

			setRegister(rd) = unsig(valueInRegister(rs1)) + unsig(valueInRegister(rs2));
		}
			break;
		case OP_SFW:
			break;
		case OP_LA:
			break;
		case OP_J:
		{
			Byte* location = findLocation();
			jumpToLocation(location);
		}
			break;
		default:
			runtimeError("Unknown instruction.");
	}
}

void initCPU(const char* header_file, const char* code_file)
{
	loadHeader(header_file);
	loadCode(code_file);
}
