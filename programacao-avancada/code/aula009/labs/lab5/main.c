#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

int main(int argc, char *argv[]) {
    /* Receber porto a registar */
    struct gengetopt_args_info args;
    cmdline_parser(argc, argv, &args);

    /* Criar e registar socket UDP IPv4 */
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        ERROR(EXIT_FAILURE, "socket() failed");
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(args.port_arg);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        ERROR(EXIT_FAILURE, "bind() failed");
    }

    printf("Socket UDP registado no porto %d\n", args.port_arg);
    // Comando para verificar portos em ipv4: netstat -u -l -n -p inet

    sleep(100); /* Manter o programa ativo por algum tempo */

    close(sockfd);

    /* Liberta memória */
    cmdline_parser_free(&args);

    return 0;
}

/*
 * Perguntas:
 *  a)  O que sucede quando é indicado um porto entre 1 e 1023 (inclusive)? 
 *      Permission denied 
 * 
 *  b)  O que sucede quando se tenta registar um porto que já está registado?
 *      Address already in use
 * 
 *      Nota: é possível obter uma lista dos portos UDP registados no sistema
 * através do utilitário netstat, executado da seguinte forma: netstat -u -l
 * 
 */
