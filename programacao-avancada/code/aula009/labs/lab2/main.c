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
void determina_endianess();

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
    determina_endianess();
}

void determina_endianess()
{
    uint16_t num = 0x00FF;
    unsigned char *byte_ptr = (unsigned char *)&num;

    if (byte_ptr[0] == 0xFF)
    {
        printf("System is Little Endian\n");
    }
    else
    {
        printf("System is Big Endian\n");
    }
}