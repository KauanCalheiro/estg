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
void memory_dump(uint32_t ip);

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
    char *address = args.address_arg;
    u_int32_t ip;

    int result = inet_pton(AF_INET, address, &ip);

    switch (result)
    {
        case -1:
            ERROR(EXIT_FAILURE, "Invalid address family");
            break;
        case 0:
            fprintf(stderr, "The address '%s' is not a valid IPv4 dotted-decimal address.\n", address);
            break;
        case 1:
            printf("The address '%s' is a valid IPv4 dotted-decimal address.\n", address);
            break;
            default:
            break;
    }

    printf("IP value: %u\n", ip);
    memory_dump(ip);
}

void memory_dump(uint32_t ip)
{
    unsigned char *byte_ptr = (unsigned char *)&ip;

    for (size_t i = 0; i < sizeof(ip); i++)
    {
        printf("[%lu] = %d\n", i,  byte_ptr[i]);
    }
}
