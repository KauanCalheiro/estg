#include <stdio.h>
#include <string.h>

int bit_n_to_zero(int input, size_t bit_to_zero);
void test(int input, size_t bit_to_zero);
char* int_to_binary_string(int input);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    test(13, 0);  // 1101 -> bit 0 to zero
    test(13, 1);  // 1101 -> bit 1 to zero
    test(13, 2);  // 1101 -> bit 2 to zero
    test(13, 3);  // 1101 -> bit 3 to zero

    return 0;
}

int bit_n_to_zero(int input, size_t bit_to_zero)
{
    int mask = ~(1 << bit_to_zero);
    return input & mask;
}

void test(int input, size_t bit_to_zero)
{
    int result = bit_n_to_zero(input, bit_to_zero);
    printf(
        "Input: %d, Bit to zero: %zu, Result: %d\n",
        input, 
        bit_to_zero,
        result
    );
}