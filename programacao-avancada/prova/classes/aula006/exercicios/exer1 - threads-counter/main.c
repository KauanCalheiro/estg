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

static int counter = 0;

typedef struct thread_data_t
{
    int sum_quantity;
    pthread_mutex_t mutex_lock;
} thread_data_t;

void app(int number_of_threads, int sum_quantity);
void *thread_process(void *arg);

int main(int argc, char *argv[])
{
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0)
    {
        return EXIT_FAILURE;
    }

    int number_of_threads = args.num_thread_arg;
    int sum_quantity      = args.sum_quantity_arg;

    app(number_of_threads, sum_quantity);

    cmdline_parser_free(&args);

    return EXIT_SUCCESS;
}

void app(int number_of_threads, int sum_quantity)
{
    pthread_t *tids = malloc(number_of_threads * sizeof(pthread_t));

    if (tids == NULL)
    {
        ERROR(ENOMEM, "malloc() failed!");
    }

    // pthread_mutex_t mutex_lock;
    struct thread_data_t params;
    params.sum_quantity = sum_quantity;

    if ((errno = pthread_mutex_init(&params.mutex_lock, NULL)) != 0)
    {
        ERROR(C_ERROR_PTHREAD_CREATE, "pthread_mutex_init() failed!");
    }

    for (int i = 0; i < number_of_threads; i++)
    {
        if ((errno = pthread_create(&tids[i], NULL, thread_process, &params)) != 0)
        {
            ERROR(C_ERROR_PTHREAD_CREATE, "pthread_create() failed!");
        }
    }

    for (int i = 0; i < number_of_threads; i++)
    {
        if ((errno = pthread_join(tids[i], NULL)) != 0)
        {
            ERROR(C_ERROR_PTHREAD_JOIN, "pthread_join() failed!\n");
        }
    }

    if ((errno = pthread_mutex_destroy(&params.mutex_lock)) != 0)
    {
        ERROR(C_ERROR_PTHREAD_CREATE, "pthread_mutex_destroy() failed!");
    }

    printf("Final counter value: %d (expected %d)\n", counter, number_of_threads * sum_quantity);

    free(tids);
}

void *thread_process(void *arg)
{
    struct thread_data_t *params = (struct thread_data_t *)arg;

    for (int i = 0; i < params->sum_quantity; i++)
    {
        if((errno = pthread_mutex_lock(&params->mutex_lock)) != 0)
        {
            ERROR(C_ERROR_PTHREAD_CREATE, "pthread_mutex_lock() failed!");
        }

        counter++;

        if((errno = pthread_mutex_unlock(&params->mutex_lock)) != 0)
        {
            ERROR(C_ERROR_PTHREAD_CREATE, "pthread_mutex_unlock() failed!");
        }
    }

    return NULL;
}