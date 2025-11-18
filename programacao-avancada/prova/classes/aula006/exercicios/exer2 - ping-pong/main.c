#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "debug.h"

#define C_ERRO_PTHREAD_CREATE 1
#define C_ERRO_PTHREAD_JOIN 2
#define C_ERRO_MUTEX_INIT 3
#define C_ERRO_MUTEX_DESTROY 4
#define C_ERRO_CONDITION_INIT 5
#define C_ERRO_CONDITION_DESTROY 6

#define LIMIT 10

typedef struct
{
    int ping;
    int pong;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} thread_params_t;

/* protótipos */
void *ping(void *arg);
void *pong(void *arg);

/* main */
int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    thread_params_t thread_params;

    (void)argc;
    (void)argv;

    /* Inicia o mutex */
    if ((errno = pthread_mutex_init(&thread_params.mutex, NULL)) != 0)
        ERROR(C_ERRO_MUTEX_INIT, "pthread_mutex_init() failed!");

    /* Inicia variavel de condicao */
    if ((errno = pthread_cond_init(&thread_params.cond, NULL)) != 0)
        ERROR(C_ERRO_CONDITION_INIT, "pthread_cond_init() failed!");

    thread_params.ping = 0;
    thread_params.pong = 1;

    /* Cria thread para executar o pong */
    if ((errno = pthread_create(&t1, NULL, pong, &thread_params)) != 0)
        ERROR(C_ERRO_PTHREAD_CREATE, "pthread_create() failed!");

    /* Cria thread para executar o ping */
    if ((errno = pthread_create(&t2, NULL, ping, &thread_params)) != 0)
        ERROR(C_ERRO_PTHREAD_CREATE, "pthread_create() failed!");

    /* Espera que todas as threads terminem */
    if ((errno = pthread_join(t1, NULL)) != 0)
        ERROR(C_ERRO_PTHREAD_JOIN, "pthread_join() failed!");

    if ((errno = pthread_join(t2, NULL)) != 0)
        ERROR(C_ERRO_PTHREAD_JOIN, "pthread_join() failed!");

    /* Destroi o mutex */
    if ((errno = pthread_mutex_destroy(&thread_params.mutex)) != 0)
        ERROR(C_ERRO_MUTEX_DESTROY, "pthread_mutex_destroy() failed!");

    /* Destroi a condicao */
    if ((errno = pthread_cond_destroy(&thread_params.cond)) != 0)
        ERROR(C_ERRO_CONDITION_DESTROY, "pthread_cond_destroy() failed!");

    return 0;
}

/* ping */
void *ping(void *arg)
{
    thread_params_t *params = (thread_params_t *)arg;
    int i;

    for (i = 0; i < LIMIT; i++)
    {
        if ((errno = pthread_mutex_lock(&(params->mutex))) != 0)
        {
            WARNING("pthread_mutex_lock() failed\n");
            return NULL;
        }

        while (params->ping == 1)
        {
            if ((errno = pthread_cond_wait(&(params->cond),
                                           &(params->mutex))) != 0)
            {
                WARNING("pthread_cond_wait() failed");
                return NULL;
            }
        }

        printf("ping...");
        params->ping = 1;
        params->pong = 0;

        if ((errno = pthread_cond_signal(&(params->cond))) != 0)
        {
            WARNING("pthread_cond_signal() failed");
            return NULL;
        }

        if ((errno = pthread_mutex_unlock(&(params->mutex))) != 0)
        {
            WARNING("pthread_mutex_unlock() failed");
            return NULL;
        }
    }
    return NULL;
}

/* pong */
void *pong(void *arg)
{
    thread_params_t *params = (thread_params_t *)arg;
    int i;

    for (i = 0; i < LIMIT; i++)
    {
        if ((errno = pthread_mutex_lock(&(params->mutex))) != 0)
        {
            WARNING("pthread_mutex_lock() failed\n");
            return NULL;
        }

        while (params->pong == 1)
        {
            if ((errno = pthread_cond_wait(&(params->cond),
                                           &(params->mutex))) != 0)
            {
                WARNING("pthread_cond_wait() failed");
                return NULL;
            }
        }

        printf("\tpong!\n");
        params->pong = 1;
        params->ping = 0;

        if ((errno = pthread_cond_signal(&(params->cond))) != 0)
        {
            WARNING("pthread_cond_signal() failed");
            return NULL;
        }

        if ((errno = pthread_mutex_unlock(&(params->mutex))) != 0)
        {
            WARNING("pthread_mutex_unlock() failed");
            return NULL;
        }
    }
    return NULL;
}
