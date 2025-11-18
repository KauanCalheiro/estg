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
    int *find;
    int *chunck;
    int chunck_size;
    int search;
    int thread_number;
} thread_data_t;

void app(struct gengetopt_args_info args);
void validate_input(struct gengetopt_args_info args);
int *get_vector(int size);
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
    validate_input(args);

    int *vector = get_vector(args.size_arg);

    pthread_t *threads = malloc(sizeof(pthread_t) * args.numthreads_arg);
    thread_data_t *data = malloc(sizeof(thread_data_t) * args.numthreads_arg);

    int chunk_size = args.size_arg / args.numthreads_arg;
    int find = 0;

    for (int i = 0; i < args.numthreads_arg; i++)
    {
        data[i].search = args.target_arg;
        data[i].chunck = &vector[i * chunk_size];
        data[i].chunck_size = chunk_size;
        data[i].thread_number = i;
        data[i].find = &find;

        if ((errno = pthread_create(&threads[i], NULL, thread_func, &data[i])) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_create() failed!");
        }
    }

    for (int i = 0; i < args.numthreads_arg; i++)
    {
        if ((errno = pthread_join(threads[i], NULL)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_join() failed!");
        }
    }

    if (find == 0)
    {
        printf("Target '%d' not found in the vector\n", args.target_arg);
    }
}

void *thread_func(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;

    for (int i = 0; i < data->chunck_size; i++)
    {
        if (data->chunck[i] == data->search)
        {
            printf(
                "Thread %d found target '%d' in position %d\n",
                data->thread_number,
                data->search,
                i + data->thread_number * data->chunck_size
            );

            *(data->find) = 1;
        }
    }

    return NULL;
}

int *get_vector(int size)
{
    unsigned int max_random_number = 3 * size;

    srand((unsigned)time(NULL));

    int *random_number_vector = malloc(sizeof(int) * size);

    if (random_number_vector == NULL)
    {
        ERROR((errno = EXIT_FAILURE), "malloc() failed for random_number_vector");
    }

    for (int i = 0; i < size; i++)
    {
        int can_use_generated_number = 1;
        int random_number = rand() % max_random_number;

        for (int j = 0; j < size; j++)
        {
            if (random_number_vector[j] == random_number)
            {
                can_use_generated_number = 0;
                break;
            }
        }

        if (can_use_generated_number == 1)
        {
            random_number_vector[i] = random_number;
        }
        else
        {
            i--;
        }
    }

    return random_number_vector;
}

void validate_input(struct gengetopt_args_info args)
{
    int size = args.size_arg;
    int numthreads = args.numthreads_arg;

    if (size % numthreads != 0)
    {
        ERROR((errno = EXIT_FAILURE), "vetor size (%d) is not multiple of numthreads (%d) \n", size, numthreads);
    }

    if (numthreads < 2)
    {
        ERROR((errno = EXIT_FAILURE), "number of threads should be larger than 1.\n");
    }
}