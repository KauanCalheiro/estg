#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "memory.h"
#include "args.h"

#define BASE_BINARY 2
#define BASE_OCTAL 8
#define BASE_HEXADECIMAL 16
#define BASE_DECIMAL 10

int base_to_decimal(const char *str, int base);
void test_base_to_decimal();

int main(int argc, char *argv[])
{
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0) {
        return EXIT_FAILURE;
    }

    test_base_to_decimal();

    cmdline_parser_free(&args);
    return EXIT_SUCCESS;
}

int base_to_decimal(const char *str, int base)
{
    char *endptr;
    long result = strtol(str, &endptr, base);

    if (*endptr != '\0' || result < 0) {
        fprintf(stderr, "Invalid input for base %d\n", base);
        return -1;
    }

    return (int)result;
}

void test_base_to_decimal()
{
    printf("Binary 1010 to Decimal: %d\n", base_to_decimal("1010", BASE_BINARY));
    printf("Octal 12 to Decimal: %d\n", base_to_decimal("12", BASE_OCTAL));
    printf("Hexadecimal A to Decimal: %d\n", base_to_decimal("A", BASE_HEXADECIMAL));
    printf("Decimal 10 to Decimal: %d\n", base_to_decimal("10", BASE_DECIMAL));
    printf("Invalid input to Decimal: %d\n", base_to_decimal("invalid", BASE_DECIMAL));
}