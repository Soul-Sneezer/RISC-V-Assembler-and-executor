#ifndef RISKV_COMMON
#define RISKV_COMMON

#include <stdint.h>

typedef struct
{
    char* name;
    uint8_t value;
} TableEntry;

// table values will be set later
TableEntry instruction_encodings[34] = {{.name = "beqz"     , .value = 0b0},
                                        {.name = "fmul.s"   , .value = 0b0},
                                        {.name = "fadd.d"   , .value = 0b0},
                                        {.name = "bnez"     , .value = 0b0},
                                        {.name = "sub"      , .value = 0b0},
                                        {.name = "fgt.s"    , .value = 0b0},
                                        {.name = "fmul.d"   , .value = 0b0},
                                        {.name = "bge"      , .value = 0b0},
                                        {.name = "fld"      , .value = 0b0},
                                        {.name = "fadd.s"   , .value = 0b0},
                                        {.name = "fsqrt.d"  , .value = 0b0},
                                        {.name = "flw"      , .value = 0b0},
                                        {.name = "ble"      , .value = 0b0},
                                        {.name = "flt.s"    , .value = 0b0},
                                        {.name = "fsub.d"   , .value = 0b0},
                                        {.name = "srai"     , .value = 0b0},
                                        {.name = "slli"     , .value = 0b0},
                                        {.name = "bgt"      , .value = 0b0},
                                        {.name = "fmv.s.x"  , .value = 0b0},
                                        {.name = "ld"       , .value = 0b0},
                                        {.name = "lb"       , .value = 0b0},
                                        {.name = "sd"       , .value = 0b0},
                                        {.name = "mv"       , .value = 0b0},
                                        {.name = "lw"       , .value = 0b0},
                                        {.name = "sb"       , .value = 0b0},
                                        {.name = "call"     , .value = 0b0},
                                        {.name = "ret"      , .value = 0b0},
                                        {.name = "add"      , .value = 0b0},
                                        {.name = "li"       , .value = 0b0},
                                        {.name = "addi"     , .value = 0b0},
                                        {.name = "fsw"      , .value = 0b0},
                                        {.name = "la"       , .value = 0b0},
                                        {.name = "fmv.s"    , .value = 0b0},
                                        {.name = "j"        , .value = 0b0}};

TableEntry reg_names[129] = {   {.name = "x0",  .value = 0 },   {.name = "zero",.value = 0 },
                                {.name = "x1",  .value = 1 },   {.name = "ra",  .value = 1 },
                                {.name = "x1",  .value = 2 },   {.name = "sp",  .value = 2 },
                                {.name = "x1",  .value = 3 },   {.name = "gp",  .value = 3 },
                                {.name = "x1",  .value = 4 },   {.name = "tp",  .value = 4 },
                                {.name = "x1",  .value = 5 },   {.name = "t0",  .value = 5 },
                                {.name = "x1",  .value = 6 },   {.name = "t1",  .value = 6 },
                                {.name = "x1",  .value = 7 },   {.name = "t2",  .value = 7 },
                                {.name = "x1",  .value = 8 },   {.name = "s0",  .value = 8 },
                                {.name = "x1",  .value = 9 },   {.name = "fp",  .value = 8 },
                                {.name = "x1",  .value = 10},   {.name = "s1",  .value = 9 },
                                {.name = "x1",  .value = 11},   {.name = "a0",  .value = 10},
                                {.name = "x1",  .value = 12},   {.name = "a1",  .value = 11},
                                {.name = "x1",  .value = 13},   {.name = "a2",  .value = 12},
                                {.name = "x1",  .value = 14},   {.name = "a3",  .value = 13},
                                {.name = "x1",  .value = 15},   {.name = "a4",  .value = 14},
                                {.name = "x1",  .value = 16},   {.name = "a5",  .value = 15},
                                {.name = "x1",  .value = 17},   {.name = "a6",  .value = 16},
                                {.name = "x1",  .value = 18},   {.name = "a7",  .value = 17},
                                {.name = "x1",  .value = 19},   {.name = "s2",  .value = 18},
                                {.name = "x1",  .value = 20},   {.name = "s3",  .value = 19},
                                {.name = "x1",  .value = 21},   {.name = "s4",  .value = 20},
                                {.name = "x1",  .value = 22},   {.name = "s5",  .value = 21},
                                {.name = "x1",  .value = 23},   {.name = "s6",  .value = 22},
                                {.name = "x1",  .value = 24},   {.name = "s7",  .value = 23},
                                {.name = "x1",  .value = 25},   {.name = "s8",  .value = 24},
                                {.name = "x1",  .value = 26},   {.name = "s9",  .value = 25},
                                {.name = "x1",  .value = 27},   {.name = "s10", .value = 26},
                                {.name = "x1",  .value = 28},   {.name = "s11", .value = 27},
                                {.name = "x1",  .value = 29},   {.name = "t3",  .value = 28},
                                {.name = "x1",  .value = 30},   {.name = "t4",  .value = 29},
                                {.name = "x1",  .value = 31},   {.name = "t5",  .value = 30},
                                {.name = "x1",  .value = 32},   {.name = "t6",  .value = 31},
                                {.name = "x1",  .value = 33},   {.name = "ft0", .value = 32},
                                {.name = "x1",  .value = 34},   {.name = "ft1", .value = 33},
                                {.name = "x1",  .value = 35},   {.name = "ft2", .value = 34},
                                {.name = "x1",  .value = 36},   {.name = "ft3", .value = 35},
                                {.name = "x1",  .value = 37},   {.name = "ft4", .value = 36},
                                {.name = "x1",  .value = 38},   {.name = "ft5", .value = 37},
                                {.name = "x1",  .value = 39},   {.name = "ft6", .value = 38},
                                {.name = "x1",  .value = 40},   {.name = "ft7", .value = 39},
                                {.name = "x1",  .value = 41},   {.name = "fs0", .value = 40},
                                {.name = "x1",  .value = 42},   {.name = "fs1", .value = 41},
                                {.name = "x1",  .value = 43},   {.name = "fa0", .value = 42},
                                {.name = "x1",  .value = 44},   {.name = "fa1", .value = 43},
                                {.name = "x1",  .value = 45},   {.name = "fa2", .value = 44},
                                {.name = "x1",  .value = 46},   {.name = "fa3", .value = 45},
                                {.name = "x1",  .value = 47},   {.name = "fa4", .value = 46},
                                {.name = "x1",  .value = 48},   {.name = "fa5", .value = 47},
                                {.name = "x1",  .value = 49},   {.name = "fa6", .value = 48},
                                {.name = "x1",  .value = 50},   {.name = "fa7", .value = 49},
                                {.name = "x1",  .value = 51},   {.name = "fs2", .value = 50},
                                {.name = "x1",  .value = 52},   {.name = "fs3", .value = 51},
                                {.name = "x1",  .value = 53},   {.name = "fs4", .value = 52},
                                {.name = "x1",  .value = 54},   {.name = "fs5", .value = 53},
                                {.name = "x1",  .value = 55},   {.name = "fs6", .value = 54},
                                {.name = "x1",  .value = 56},   {.name = "fs7", .value = 55},
                                {.name = "x1",  .value = 57},   {.name = "fs8", .value = 56},
                                {.name = "x1",  .value = 58},   {.name = "fs9", .value = 57},
                                {.name = "x1",  .value = 59},   {.name = "fs10",.value = 58},
                                {.name = "x1",  .value = 60},   {.name = "fs11",.value = 59},
                                {.name = "x1",  .value = 61},   {.name = "ft8", .value = 60},
                                {.name = "x1",  .value = 62},   {.name = "ft9", .value = 61},
                                {.name = "x1",  .value = 63},   {.name = "ft10",.value = 62},
                                {.name = "ft11",.value = 63}};

#define GROW_LIST(x) ( (x) == 0 ? 8 : (2 * (x)))

int32_t parseNumber(char* buffer, int32_t* index);
char* readFile(const char* path);
void toLowercaseC(char* c);
void toLowercase(char** s);
void copyWord(char** from, char** to, int32_t length);

#endif
