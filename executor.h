typedef struct 
{
	unsigned int bit : 1;
} Bit;

typedef struct 
{
	Bit bits[8];
} Byte;

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

void readFile();
void initMemory(Memory* mem);
void freeMemory(Memory* mem);
void initCPU(CPU* cpu);
void freeCPU(CPU* cpu);
void loadCode(CPU* cpu);
void loadStack(CPU* cpu);
void loadHeap(CPU* cpu);
Byte getByte();
