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
#include "handle_fork.h"

typedef struct
{
    char *title;
    int count;
} parent_arg_t;

typedef struct
{
    char *message;
    int count;
} child_arg_t;

void parent_fn(void *arg);
void child_fn(void *arg);

void parent_fn(void *arg)
{
    parent_arg_t *parent_arg = (parent_arg_t *)arg;
    printf("Parent PID: %d\n", getpid());
    printf("Parent Arg Title: %s\n", parent_arg->title);
    printf("Parent Arg Count: %d\n", parent_arg->count);
}

void child_fn(void *arg)
{
    child_arg_t *child_arg = (child_arg_t *)arg;
    printf("Child PID: %d\n", getpid());
    printf("Child Arg Message: %s\n", child_arg->message);
    printf("Child Arg Count: %d\n", child_arg->count);

    child_arg->message = "Child process modified this message";
}

int main(int argc, char *argv[])
{
    /* Initialize argument structure */
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0)
    {
        return EXIT_FAILURE;
    }

    printf("Main process PID: %d\n\n", getpid());

    pid_t pid = fork();

    parent_arg_t parent_arg = { .title = "Parent Process", .count = 1 };
    child_arg_t child_arg = { .message = "Hello from Child Process", .count = 1 };

    handle_fork(pid, parent_fn, &parent_arg, child_fn, &child_arg);

    printf("Child Arg Message: %s\n", child_arg.message);

    cmdline_parser_free(&args);

    return EXIT_SUCCESS;
}
