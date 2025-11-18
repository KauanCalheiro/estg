#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

typedef struct
{
    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
} thread_data_t;

void app(struct gengetopt_args_info args);
void *thread_func(void *arg);

int main(int argc, char *argv[])
{
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0)
    {
        return EXIT_FAILURE;
    }

    app(args);

    cmdline_parser_free(&args);
    return EXIT_SUCCESS;
}

void app(struct gengetopt_args_info args)
{
    // char *string = args.str_arg;

    pthread_mutex_t mutex;
    if ((errno = pthread_mutex_init(&mutex, NULL)) != 0)
    {
        ERROR(EXIT_FAILURE, "pthread_mutex_init() failed!");
    }

    pthread_cond_t cond;
    if ((errno = pthread_cond_init(&cond, NULL)) != 0)
    {
        ERROR(EXIT_FAILURE, "pthread_cond_init() failed!");
    }

    pthread_t threads[26];
    thread_data_t data[26];

    for (int i = 0; i < 26; i++)
    {
        data[i].mutex = &mutex;
        data[i].cond = &cond;

        if ((errno = pthread_create(&threads[i], NULL, thread_func, &data[i])) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_create() failed!");
        }
    }

    for (int i = 0; i < 26; i++)
    {
        if ((errno = pthread_join(threads[i], NULL)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_join() failed!");
        }
    }

    if ((errno = pthread_mutex_destroy(&mutex)) != 0)
    {
        ERROR(EXIT_FAILURE, "pthread_mutex_destroy() failed!");
    }

    if ((errno = pthread_cond_destroy(&cond)) != 0)
    {
        ERROR(EXIT_FAILURE, "pthread_cond_destroy() failed!");
    }
}

void *thread_func(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;

    while (1)
    {
        // ESPERA PELO LOCK
        if ((errno = pthread_mutex_lock(data->mutex)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_mutex_lock() failed\n");
        }

        while (1)
        {
            // ESPERA ATÉ SER A SUA VEZ DE ESCREVER
            if ((errno = pthread_cond_wait(data->cond, data->mutex)) != 0)
            {
                ERROR(EXIT_FAILURE, "pthread_cond_wait() failed");
            }
        }

        if (1)
        {
            // ACORDA TODAS AS THREADS EM "COND WAIT"
            if ((errno = pthread_cond_broadcast(data->cond)) != 0)
            {
                ERROR(EXIT_FAILURE, "pthread_cond_broadcast() failed");
            }

            // // ACORDA UMA THREAD ESPECÍFICA EM "COND WAIT"
            // if ((errno = pthread_cond_signal(data->cond)) != 0)
            // {
            //     ERROR(EXIT_FAILURE, "pthread_cond_signal() failed");
            // }
        }

        // LIBERTA O LOCK
        if ((errno = pthread_mutex_unlock(data->mutex)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_mutex_unlock() failed");
        }

        // SAÍDA DO LOOP INFINITO
        if (1)
        {
            break;
        }
    }

    return NULL;
}