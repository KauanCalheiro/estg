#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/socket.h>

#include "debug.h"
#include "memory.h"

int main(void) {
    /* a) Criar um socket UDP / IPv4 */
    int sockfd_ipv4 = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ipv4 < 0) {
        ERROR(EXIT_FAILURE, "socket() failed");
    }

    /* b) Criar um socket UDP / IPv6 */
    int sockfd_ipv6 = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd_ipv6 < 0) {
        ERROR(EXIT_FAILURE, "socket() failed");
    }

    /* c) Mostrar descritores dos sockets */
    printf("Socket UDP IPv4: %d\n", sockfd_ipv4); // 3
    printf("Socket UDP IPv6: %d\n", sockfd_ipv6); // 4

    /* d) Fechar os descritores dos sockets */
    close(sockfd_ipv4);
    close(sockfd_ipv6);

    return 0;
}
