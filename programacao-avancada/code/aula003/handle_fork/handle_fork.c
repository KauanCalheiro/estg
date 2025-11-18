#include "handle_fork.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


void handle_fork(pid_t pid, proc_func parent_cb, void *parent_arg,
                 proc_func child_cb, void *child_arg)
{
    switch (pid)
    {
        case -1:
            perror("fork failed");
            exit(EXIT_FAILURE);

        case 0:
            if (child_cb)
                child_cb(child_arg);
            break;

        default:
            if (parent_cb)
                parent_cb(parent_arg);
            break;
    }
}
