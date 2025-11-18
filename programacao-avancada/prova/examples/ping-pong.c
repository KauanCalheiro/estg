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

#define LIMIT 10
#define NUM_THREADS 2
#define PING_THREAD 0
#define PONG_THREAD 1

typedef struct
{
    int thread_id;
    int *ping;
    int *pong;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
} thread_data_t;

void app(struct gengetopt_args_info args);
void *ping_pong(void *arg);

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
    (void)args;

    pthread_t threads[2];
    thread_data_t thread_data[2];

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

    int ping = 1;
    int pong = 0;

    thread_data[0].thread_id = PING_THREAD;
    thread_data[0].mutex = &mutex;
    thread_data[0].cond = &cond;
    thread_data[0].ping = &ping;
    thread_data[0].pong = &pong;

    thread_data[1].thread_id = PONG_THREAD;
    thread_data[1].mutex = &mutex;
    thread_data[1].cond = &cond;
    thread_data[1].ping = &ping;
    thread_data[1].pong = &pong;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if ((errno = pthread_create(&threads[i], NULL, ping_pong, &thread_data[i])) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_create() failed!");
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
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

void *ping_pong(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;

    for (int i = 0; i < LIMIT; i++)
    {
        if ((errno = pthread_mutex_lock(data->mutex)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_mutex_lock() failed\n");
        }

        if(data->thread_id == PING_THREAD)
        {
            while (*(data->ping) == 0)
            {
                if ((errno = pthread_cond_wait(data->cond, data->mutex)) != 0)
                {
                    ERROR(EXIT_FAILURE, "pthread_cond_wait() failed");
                }
            }

            printf("ping...\n");
            *(data->ping) = 0;
            *(data->pong) = 1;

            // Poderia ser pthread_cond_broadcast, mas como so ha uma thread a esperar, pode ser signal
            if ((errno = pthread_cond_signal(data->cond)) != 0)
            {
                ERROR(EXIT_FAILURE, "pthread_cond_signal() failed");
            }
        }

        if(data->thread_id == PONG_THREAD)
        {
            while (*(data->pong) == 0)
            {
                if ((errno = pthread_cond_wait(data->cond, data->mutex)) != 0)
                {
                    ERROR(EXIT_FAILURE, "pthread_cond_wait() failed");
                }
            }

            printf("\tpong!\n");
            *(data->pong) = 0;
            *(data->ping) = 1;

            if ((errno = pthread_cond_signal(data->cond)) != 0)
            {
                ERROR(EXIT_FAILURE, "pthread_cond_signal() failed");
            }
        }

        if ((errno = pthread_mutex_unlock(data->mutex)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_mutex_unlock() failed");
        }
    }

    return NULL;
}