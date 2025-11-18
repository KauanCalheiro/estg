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

#define MIN_VAL 1
#define MAX_VAL 100

typedef struct
{
    int size;
    int *vector;
} vector_thread_t;

typedef struct
{
    int *chunk_vector;
    int *partial_sum;
    int size;
} partial_thread_t;

typedef struct
{
    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
} final_thread_t;

void app(struct gengetopt_args_info args);
void validate_input(struct gengetopt_args_info args);
void soma_final(int *vector, int size, int final_sum);

void *thread_create_vector(void *arg);
void *thread_partial_sum(void *arg);
void *thread_final_sum(void *arg);

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
    validate_input(args);

    printf("sum_parallel: tamanho=%d threads=%d\n", args.size_arg, args.threads_arg);

    // criar thread de vetor
    pthread_t vector_thread;
    vector_thread_t vector_thread_data;

    vector_thread_data.size = args.size_arg;

    int *vector = malloc(sizeof(int) * args.size_arg);
    vector_thread_data.vector = vector;

    if ((errno = pthread_create(&vector_thread, NULL, thread_create_vector, &vector_thread_data)) != 0)
    {
        ERROR(EXIT_FAILURE, "pthread_create() failed!");
    }

    if ((errno = pthread_join(vector_thread, NULL)) != 0)
    {
        ERROR(EXIT_FAILURE, "pthread_join() failed!");
    }

    pthread_t *patial_thread = malloc(sizeof(pthread_t) * args.threads_arg);
    partial_thread_t *patial_thread_data = malloc(sizeof(partial_thread_t) * args.threads_arg);

    // int chunks_size = args.size_arg / args.threads_arg;
    // int chunks = args.size_arg / chunks_size;
    soma_final(vector, args.size_arg, 10);

    
}

void validate_input(struct gengetopt_args_info args)
{
    if (args.size_arg < 1)
    {
        ERROR((errno = EXIT_FAILURE), "argumento inválido --size (deve ser >= 1)");
    }

    if (args.threads_arg < 1)
    {
        ERROR((errno = EXIT_FAILURE), "argumento inválido --threads (deve ser >= 1)");
    }
}

void *thread_create_vector(void *arg)
{
    vector_thread_t *data = (vector_thread_t *)arg;

    unsigned int seed = time(NULL) ^ getpid();

    for (int i = 0; i < data->size; i++)
    {
        data->vector[i] = (MIN_VAL + rand_r(&seed) % (MAX_VAL - MIN_VAL + 1));
    }

    return NULL;
}

void *thread_partial_sum(void *arg)
{
    partial_thread_t *data = (partial_thread_t *)arg;

    for (int i = 0; i < data->size; i++)
    {
        *data->partial_sum = data->chunk_vector[i];
    }

    return NULL;
}

void soma_final(int *vector, int size, int final_sum)
{
    int sum = 0;

    for (int i = 0; i < size; i++)
    {
        sum += vector[i];
    }

    printf("main: verificação sequencial=%d\n", sum);

    if (sum == final_sum)
    {
        printf("validação: OK\n");
    }
    else
    {
        printf("Erro na soma final\n");
    }
}