#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "memory.h"
#include "debug.h"
#include "common.h"
#include "client_opt.h"

int main(int argc, char *argv[])
{
    /* Estrutura gerada pelo utilitario gengetopt */
    struct gengetopt_args_info args_info;

    /* Processa os parametros da linha de comando */
    if (cmdline_parser(argc, argv, &args_info) != 0)
    {
        exit(ERR_ARGS);
    }

    const char *ip = args_info.ip_arg;

    int port = args_info.port_arg;
    if (port <= 0 || port > 65535)
    {
        fprintf(stderr, "Invalid port number: %d\n", port);
        exit(ERR_ARGS);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    switch (inet_pton(AF_INET, ip, &server_addr.sin_addr))
    {
        case 0:
            fprintf(stderr, "Invalid IP address: %s\n", ip);
            exit(ERR_ARGS);
            break;
        case -1:
            ERROR(1, "inet_pton() failed");
            break;
    }

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        fprintf(stderr, "Invalid IP address: %s\n", ip);
        exit(ERR_ARGS);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        ERROR(1, "socket() failed");
    }

    srand(time(NULL) ^ getpid());
    uint16_t valor = (rand() % 100) + 1;

    printf("[CLIENT] Enviando %d para %s:%d...\n", valor, ip, port);

    valor = htons(valor);

    ssize_t nsent = sendto(sock, &valor, sizeof(valor), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (nsent < 0)
    {
        ERROR(1, "sendto() failed");
    }

    char buffer[MAX_BUFFER + 1];
    socklen_t server_addr_len = sizeof(server_addr);
    ssize_t nread = recvfrom(sock, buffer, MAX_BUFFER, 0, (struct sockaddr *)&server_addr, &server_addr_len);
    if (nread < 0)
    {
        ERROR(1, "recvfrom() failed");
    }
    buffer[nread] = '\0';

    printf("[CLIENT] Raiz quadrada recebida: %s\n", buffer);

    close(sock);

    cmdline_parser_free(&args_info);

    return 0;
}
