#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

#define C_ERROR_PTHREAD_CREATE 1
#define C_ERROR_PTHREAD_JOIN 2

#define NUMBER_OF_CHILD_PROCESSES 3
#define NUMBER_OF_THREADS_PER_CHILD 2

void handle_fork();
void child_fork_process();
void *thread_process(void *arg);

int main(int argc, char *argv[])
{
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0)
    {
        return EXIT_FAILURE;
    }

    handle_fork();

    cmdline_parser_free(&args);

    return EXIT_SUCCESS;
}

void handle_fork()
{
    for (size_t i = 0; i < NUMBER_OF_CHILD_PROCESSES; i++)
    {
        pid_t pid = fork();

        switch (pid)
        {
            case -1:
                perror("fork failed");
                exit(EXIT_FAILURE);

            case 0:
                child_fork_process();
                exit(EXIT_SUCCESS);
            default:
                // Parent process
                break;
        }
    }

    for (size_t i = 0; i < NUMBER_OF_CHILD_PROCESSES; i++)
    {
        wait(NULL);
    }
}

void child_fork_process()
{
    pthread_t tids[NUMBER_OF_THREADS_PER_CHILD];

    for (int i = 0; i < NUMBER_OF_THREADS_PER_CHILD; i++)
    {
        if ((errno = pthread_create(&tids[i], NULL, thread_process, NULL)) != 0)
        {
            ERROR(C_ERROR_PTHREAD_CREATE, "pthread_create() failed!");
        }
    }

    for (int i = 0; i < NUMBER_OF_THREADS_PER_CHILD; i++)
    {
        if ((errno = pthread_join(tids[i], NULL)) != 0)
        {
            ERROR(C_ERROR_PTHREAD_JOIN, "pthread_join() failed!\n");
        }
    }
}

void *thread_process(void *arg)
{
    (void)arg;

    printf(
        "Parent PID: %d, Process PID: %d, Thread TID: %lu\n",
        getppid(),
        getpid(),
        pthread_self()
    );

    return NULL;
}