/**
 * @file main.c
 * @brief Description
 * @date 2018-1-1
 * @author name of author
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <arpa/inet.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

void app(struct gengetopt_args_info args);
void memory_dump(const void *data, size_t size);

int main(int argc, char *argv[])
{
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0)
    {
        return EXIT_FAILURE;
    }

    app(args);

    cmdline_parser_free(&args);
    return EXIT_SUCCESS;
}

void app(struct gengetopt_args_info args)
{
    (void)args;

    struct sockaddr_in my_addr_IPv4;

    // struct sockaddr_in my_addr_IPv4 = { 0 }; // Alternativa para inicializar a zero a estrutura

    // Mostra o tamanho da estrutura struct sockaddr_in;
    printf("Size of struct sockaddr_in: %zu bytes\n", sizeof(struct sockaddr_in));

    // Fazendo uso da função memset, coloca a zero a zona de memória ocupada por uma variável do tipo struct sockaddr_in (my_addr_IPv4);
    memset(&my_addr_IPv4, 0, sizeof(my_addr_IPv4));

    // Após a chamada à função memset, usa a função memory_dump criada no lab1 para mostrar cada um dos bytes da zona de memória ocupada pela variável my_addr_IPv4, confirmando que cada byte tem o valor zero;
    // Sugestão: iterar a zona de memória da variável do tipo struct sockaddr_in com um ponteiro para unsigned char (unsigned char *).
    memory_dump(&my_addr_IPv4, sizeof(my_addr_IPv4));
    printf("\n");

    // ​ Fazendo uso da função memset, coloca o valor 4 em cada um dos primeiros 6 bytes da variável my_addr_IPv4;
    memset(&my_addr_IPv4, 4, 6);

    // ​ Utiliza a função memory_dump para voltar a mostrar o estado atual da zona de memória da variável my_addr_IPv4.
    memory_dump(&my_addr_IPv4, sizeof(my_addr_IPv4));
}

void memory_dump(const void *data, size_t size)
{
    unsigned char *byte_ptr = (unsigned char *)data;

    for (size_t i = 0; i < size; i++)
    {
        printf("[%lu] = %d\n", i,  byte_ptr[i]);
    }
}
