#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int bin_to_decimal(const char *bin_s_ptr);
void test(const char *bin_s_ptr);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    test("1101");
    test("10010");
    test("11111111");
    test("00001");
    test("00000");

    return 0;
}

int bin_to_decimal(const char *bin_s_ptr)
{
    char *endptr;
    long result = strtol(bin_s_ptr, &endptr, 2);

    if (*endptr != '\0' || result < 0) {
        fprintf(stderr, "Invalid input for base %d\n", 2);
        return -1;
    }

    return (int)result;
}

void test(const char *bin_s_ptr)
{
    int result = bin_to_decimal(bin_s_ptr);
    printf("Binary string: %s\n", bin_s_ptr);
    printf("Decimal value: %d\n\n", result);
}