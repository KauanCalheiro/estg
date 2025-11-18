#include <stdio.h>
#include <string.h>

int is_bit_n_set(int input, size_t n);

void test(int input, size_t n, int expected);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    test(13, 0, 1);  // 1101 -> bit 0 is set
    test(13, 1, 0);  // 1101 -> bit 1 is not set
    test(13, 2, 1);  // 1101 -> bit 2 is set
    test(13, 3, 1);  // 1101 -> bit 3 is set
    test(18, 1, 1);  // 10010 -> bit 1 is set
    test(18, 4, 1);  // 10010 -> bit 4 is set

    return 0;
}

int is_bit_n_set(int input, size_t n)
{
    int mask = 1 << n;
    return input & mask;
}


void test(int input, size_t n, int expected)
{
    int result = is_bit_n_set(input, n);
    printf("Input: %d, Bit position: %zu\n", input, n);
    printf("In binary: ");
    for (size_t i = sizeof(input) * 8; i > 0; i--)
    {
        printf("%d", (input >> (i - 1)) & 1);
    }
    printf("\n");
    printf("Is bit %zu set? %s\n", n, result ? "Yes" : "No");
    printf("Expected: %s\n\n", expected ? "Yes" : "No");
}