#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

void app(struct gengetopt_args_info args);
void *write_letter(void *arg);
int index_letter(char letter);
void validate_string(const char *str);

typedef struct
{
    char letter;
    int wait;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} thread_data_t;

thread_data_t *thread_data[26];
int current_index = 0;
char *string;
pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;

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
    string = args.str_arg;
    validate_string(string);

    printf("String recebida: %s\n", string);

    int i;
    pthread_t threads[26];

    for (i = 0; i < 26; i++)
    {
        thread_data[i] = malloc(sizeof(thread_data_t));
        thread_data_t *data = thread_data[i];

        if (data == NULL)
        {
            ERROR(EXIT_FAILURE, "malloc() failed!");
        }

        data->letter = 'a' + i;
        data->wait = 1;

        if ((errno = pthread_mutex_init(&data->mutex, NULL)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_mutex_init() failed!");
        }

        if ((errno = pthread_cond_init(&data->cond, NULL)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_cond_init() failed!");
        }

        if ((errno = pthread_create(&threads[i], NULL, write_letter, (void *)data)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_create() failed!");
        }
    }

    int first_index = index_letter(string[0]);
    if (first_index != -1)
    {
        pthread_mutex_lock(&thread_data[first_index]->mutex);
        thread_data[first_index]->wait = 0;
        pthread_cond_signal(&thread_data[first_index]->cond);
        pthread_mutex_unlock(&thread_data[first_index]->mutex);
    }

    pthread_mutex_lock(&global_mutex);
    while (current_index < strlen(string))
    {
        pthread_mutex_unlock(&global_mutex);
        pthread_mutex_lock(&global_mutex);
    }
    pthread_mutex_unlock(&global_mutex);

    for (i = 0; i < 26; i++)
    {
        pthread_mutex_lock(&thread_data[i]->mutex);
        thread_data[i]->wait = 0;
        pthread_cond_signal(&thread_data[i]->cond);
        pthread_mutex_unlock(&thread_data[i]->mutex);
    }

    for (i = 0; i < 26; i++)
    {
        if ((errno = pthread_join(threads[i], NULL)) != 0)
        {
            ERROR(EXIT_FAILURE, "pthread_join() failed!");
        }
    }

    for (i = 0; i < 26; i++)
    {
        pthread_mutex_destroy(&thread_data[i]->mutex);
        pthread_cond_destroy(&thread_data[i]->cond);
        free(thread_data[i]);
    }

    printf("String escrita: %s\n", string);
}

int index_letter(char letter)
{
    if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }
    else if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A';
    }
    return -1;
}

void validate_string(const char *str)
{
    for (size_t i = 0; i < strlen(str); i++)
    {
        char c = str[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
        {
            errno = 1;
            ERROR(EXIT_FAILURE, "String contains invalid character\n");
        }
    }
}

void *write_letter(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;

    while (1)
    {
        pthread_mutex_lock(&data->mutex);

        while (data->wait)
        {
            pthread_cond_wait(&data->cond, &data->mutex);
        }

        pthread_mutex_lock(&global_mutex);

        if (current_index >= strlen(string))
        {
            pthread_mutex_unlock(&global_mutex);
            pthread_mutex_unlock(&data->mutex);
            break;
        }

        char current_char = string[current_index];
        printf("Thread ID: %lu, Letter: %c\n", pthread_self(), current_char);

        current_index++;

        if (current_index < strlen(string))
        {
            int next_index = index_letter(string[current_index]);

            if (next_index != -1)
            {
                if (next_index == index_letter(data->letter))
                {
                    data->wait = 0;
                }
                else
                {
                    pthread_mutex_lock(&thread_data[next_index]->mutex);
                    thread_data[next_index]->wait = 0;
                    pthread_cond_signal(&thread_data[next_index]->cond);
                    pthread_mutex_unlock(&thread_data[next_index]->mutex);
                    data->wait = 1;
                }
            }
        }

        pthread_mutex_unlock(&global_mutex);
        pthread_mutex_unlock(&data->mutex);
    }

    return NULL;
}