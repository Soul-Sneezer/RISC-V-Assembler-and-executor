#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "parser.h"
#include "vm.h"
#include "common.h"

typedef enum
{
	SUCCESS,
	COMPILE_ERROR,
	RUNTIME_ERROR,
} ErrorType;

ErrorType runFile(const char* source)
{
}

int main(int argc, const char* argv[])
{
	if (argc == 2)
	{
		const char* source = readFile(argv[1]);
		Scanner* scanner = initScanner(source);
		freeScanner(scanner);
	}
	else
	{
		fprintf(stderr, "Usage: risk [path]\n");
		exit(64);
	}

	return 0;
}
