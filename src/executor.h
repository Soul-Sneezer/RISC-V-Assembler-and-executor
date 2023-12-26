#ifndef RISKV_EXECUTOR
#define RISKV_EXECUTOR

#include <stdint.h>

typedef Byte uint8_t;

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
	Byte mem[8192];
	Byte* base_pointer;
	Byte* stack_top;
	Byte* heap;
	Byte* code;
} Memory;

typedef struct 
{
	Memory cpu_mem;
	DWord registers[32];
} CPU;

void initCPU(CPU* cpu);
void freeCPU(CPU* cpu);
void execute(CPU* cpu);

#endif
