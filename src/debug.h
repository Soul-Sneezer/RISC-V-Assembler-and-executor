#ifndef RISKV_DEBUG
#define RISKV_DEBUG

#include "scanner.h"
#include "vm.h"

void tokenInfo(Token token);
void dumpStack(VM* vm);
void dumpHeap(VM* vm);
void dumpCode(VM* vm);
void dumpRegisters(VM* vm);
void showState(VM* vm);

#endif
