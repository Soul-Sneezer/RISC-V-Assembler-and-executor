#ifndef RISKV_EXECUTOR
#define RISKV_EXECUTOR

#include <stdint.h>

typedef enum
{
	OP_BEQZ, OP_FMULS, OP_FADDD, OP_BNEZ, OP_SUB, OP_FGT, OP_FMULD,
	OP_BGE, OP_FLD, OP_FADDS, OP_FSQRT, OP_FLW, OP_BLE, OP_FLT, OP_FSUB, OP_SRAI,
	OP_SLLI, OP_BGT, OP_FMV, OP_LD, OP_LB, OP_SD, OP_MV, OP_LW, OP_SB, 
	OP_CALL, OP_RET, OP_ADD, OP_LI, OP_ADDI, OP_SFW, OP_LA, OP_J,
} Opcodes;

typedef uint8_t Byte;

typedef struct
{
	Byte bytes[2];
} Word;

typedef struct
{
	Byte bytes[4];
} DWord;

typedef struct
{
	char* name;
	Byte* entry_point;
} Location;

typedef struct 
{
	Byte mem[8192];
	Byte* base_pointer;
	Byte* stack_top;
	Byte* heap;
	Location* labels;
	Byte* code;
} Memory;

typedef struct 
{
	Memory cpu_mem;
	DWord registers[64];
} VM;

void loadHeader(VM* cpu, const char* header_file);
void loadCode(VM* cpu, const char* code_file);
void initCPU(const char* header_file, const char* code_file);
void freeCPU(VM* cpu);
void execute(VM* cpu);

#endif
