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

void *thread_process(void *arg);
void fork_child(int *parent_pid);

int main(int argc, char *argv[])
{
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0)
    {
        return EXIT_FAILURE;
    }

    int parent_pid = getpid();

    for (size_t i = 0; i < NUMBER_OF_CHILD_PROCESSES; i++)
    {
        pid_t pid = fork();

        switch (pid)
        {
            case -1:
                perror("fork failed");
                exit(EXIT_FAILURE);

            case 0:
                fork_child(&parent_pid);
                return EXIT_SUCCESS;
            default:
                // Parent process
                break;
        }
    }

    for (size_t i = 0; i < NUMBER_OF_CHILD_PROCESSES; i++)
    {
        wait(NULL);
    }

    cmdline_parser_free(&args);

    return EXIT_SUCCESS;
}

void fork_child(int *parent_pid)
{
    pthread_t tids[NUMBER_OF_THREADS_PER_CHILD];

    void *thread_args = (void *)(int *)parent_pid;

    for (int i = 0; i < NUMBER_OF_THREADS_PER_CHILD; i++)
    {
        if ((errno = pthread_create(&tids[i], NULL, thread_process, &thread_args)) != 0)
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
    int parent_pid = *(int *)arg;

    printf("Parent PID: %d, Process PID: %d, Thread TID: %lu\n",
           parent_pid, getpid(), pthread_self());
    // printf("Parent PID: %d\n", parent_pid);
    // printf("Process PID: %d\n", getpid());
    // printf("Thread TID: %lu\n", pthread_self());
    // printf("\n");

    return NULL;
}