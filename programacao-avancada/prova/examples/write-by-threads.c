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

void app(struct gengetopt_args_info args);
void *write_letter(void *arg);
void validate_input(char *input_string);

typedef struct
{
    char letter;
    char *string;
    size_t *current_position;
    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
} thread_data_t;

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
    char *string = args.str_arg;

    validate_input(string);

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

    size_t current_position = 0;
    pthread_t threads[26];
    thread_data_t data[26];

    for (int i = 0; i < 26; i++)
    {
        data[i].string = string;
        data[i].mutex = &mutex;
        data[i].cond = &cond;
        data[i].letter = 'a' + i;
        data[i].current_position = &current_position;

        if ((errno = pthread_create(&threads[i], NULL, write_letter, &data[i])) != 0)
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

void *write_letter(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;

    while (1)
    {
        if ((errno = pthread_mutex_lock(data->mutex)) != 0)
        {
            ERROR(1, "pthread_mutex_lock() failed\n");
        }

        int is_same_letter     = tolower(data->string[*data->current_position]) == data->letter;
        int is_string_finished = *data->current_position >= strlen(data->string);

        while (!is_string_finished && !is_same_letter)
        {
            if ((errno = pthread_cond_wait(data->cond, data->mutex)) != 0)
            {
                ERROR(1, "pthread_cond_wait() failed");
            }

            is_same_letter     = tolower(data->string[*data->current_position]) == data->letter;
            is_string_finished = *data->current_position >= strlen(data->string);
        }

        if (is_same_letter)
        {
            printf(
                "%c [thread '%c'] (ID=%lu)\n",
                data->string[*data->current_position],
                data->letter,
                pthread_self()
            );

            *data->current_position = *data->current_position + 1;

            if ((errno = pthread_cond_broadcast(data->cond)) != 0)
            {
                ERROR(1, "pthread_cond_broadcast() failed");
                return NULL;
            }
        }

        if (is_string_finished)
        {
            if ((errno = pthread_mutex_unlock(data->mutex)) != 0)
            {
                ERROR(1, "pthread_mutex_unlock() failed");
            }
            break;
        }

        if ((errno = pthread_mutex_unlock(data->mutex)) != 0)
        {
            ERROR(1, "pthread_mutex_unlock() failed");
        }
    }

    return NULL;
}

void validate_input(char *input_string)
{
    for (size_t i = 0; i < strlen(input_string); i++)
    {
        if (!isalpha(input_string[i]))
        {
            errno = 1;
            ERROR(EXIT_FAILURE, "Invalid string! Only alphabetic characters (a-z, A-Z) are allowed.");
        }
    }
}