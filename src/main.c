#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "parser.h"
//#include "vm.h"
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

int32_t main(int32_t argc, const char* argv[])
{
    if (argc == 2)
    {
        const char* source = readFile(argv[1]);
        Scanner* scanner = initScanner(source);

        Parser* parser = initParser(instruction_names, instruction_values, register_names, register_values, "header.kelp", "code.ok", instructions_size, registers_size);

        parse(parser, scanner);
        freeScanner(scanner);
        freeParser(parser);
    }
    else
    {
        fprintf(stderr, "Usage: risk [path]\n");
        exit(64);
    }

    return 0;
}
