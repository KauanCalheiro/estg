#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "debug.h"
#include "common.h"
#include "server_opt.h"

int main(int argc, char *argv[])
{
    /* Estrutura gerada pelo utilitario gengetopt */
    struct gengetopt_args_info args_info;

    /* Processa os parametros da linha de comando */
    if (cmdline_parser(argc, argv, &args_info) != 0)
    {
        exit(ERR_ARGS);
    }

    int port = args_info.port_arg;
    if (port <= 0 || port > 65535)
    {
        fprintf(stderr, "Invalid port number: %d\n", port);
        exit(ERR_ARGS);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        ERROR(1, "socket() failed");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        ERROR(1, "bind() failed");
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int16_t buffer;
    while (1)
    {
        ssize_t nread = recvfrom(sock, &buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (nread < 0)
        {
            ERROR(1, "recvfrom() failed");
        }

        buffer = ntohs(buffer);
        printf("[SERVER] Calcular raiz quadrada de %d...\n", buffer);
        double result = sqrt((double)buffer);

        char response[MAX_BUFFER + 1];
        sprintf(response, "%.3f", result);

        ssize_t nsend = sendto(sock, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_addr_len);
        if (nsend < 0)
        {
            ERROR(1, "sendto() failed");
        }
    }

    cmdline_parser_free(&args_info);

    return 0;
}
