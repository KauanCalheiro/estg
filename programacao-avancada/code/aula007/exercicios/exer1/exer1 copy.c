#include <stdio.h>
#include <string.h>

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
    int decimal = 0;
    int length = strlen(bin_s_ptr) - 1;

    for (int i = length, j = 0; i >= 0; i--, j++)
    {
        char bit = bin_s_ptr[i];

        if (bit == '1')
        {
            decimal += (1 << j);
        }
    }

    return decimal;
}

void test(const char *bin_s_ptr)
{
    int result = bin_to_decimal(bin_s_ptr);
    printf("Binary string: %s\n", bin_s_ptr);
    printf("Decimal value: %d\n\n", result);
}