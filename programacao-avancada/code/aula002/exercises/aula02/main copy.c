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

#include "debug.h"
#include "memory.h"
#include "args.h" // include the generated header

int main(int argc, char *argv[]) {
    struct gengetopt_args_info args;

    // parse command line
    if (cmdline_parser(argc, argv, &args) != 0) {
        return 1;
    }

    int num_procs = args.num_procs_arg; // get the value of the option 

    for (int i = 0; i < num_procs; i++) {
        pid_t pid = fork();
        switch (pid) {
            case -1:
                // error
                printf("fork failed: %s", strerror(errno));
                ERROR(1, "fork() failed");
                break;
            case 0:
                // child
                // child has to do something and then exit
                printf("Child process %d with PID %d\n", i + 1, getpid());
                return 0;
            default:
                // parent
                // parent should do nothing and continue the loop
                break;
        }
    }

    for (int i = 0; i < num_procs; i++) {
        wait(NULL); // wait for all child processes to finish
    }

    cmdline_parser_free(&args); // free allocated memory

    return 0;
}
