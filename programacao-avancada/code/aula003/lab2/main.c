#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

int main(int argc, char *argv[]) {
    /* Initialize argument structure */
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0) {
        return 1;
    }

    // printf("Numero de threads: %d\n\n", sysconf(_SC_NPROCESSORS_ONLN));

    printf("Nome do programa: %s\n", argv[0]);
    printf("PID do processo actual: %d\n\n", getpid());
    // printf("Numero de threads:");
    // system("ps -eLf | grep terminal | grep -v grep | wc -l");
    // a mensagem a cima fica desordendada por causa do buffer do stdout
    printf("Numero de threads: ");
    fflush(stdout);
    // system("ps -eLf | grep terminal | grep -v grep | wc -l");
    char command[100];
    snprintf(command, "ps -eLf | grep %d | grep -v grep | wc -l", getpid());
    system(command);

    cmdline_parser_free(&args);
    return 0;
}
