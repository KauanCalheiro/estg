#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "debug.h"
#include "args.h"

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int ready;
} thread_sync_t;

thread_sync_t sync[26];
char *input_string;
size_t current_pos = 0;
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int char_to_index(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a';
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    return -1;
}

void *thread_function(void *arg)
{
    int my_index = *(int *)arg;
    free(arg);

    while (1)
    {
        pthread_mutex_lock(&sync[my_index].mutex);
        while (!sync[my_index].ready)
        {
            pthread_cond_wait(&sync[my_index].cond, &sync[my_index].mutex);
        }
        sync[my_index].ready = 0;
        pthread_mutex_unlock(&sync[my_index].mutex);

        pthread_mutex_lock(&global_lock);
        if (current_pos >= strlen(input_string))
        {
            pthread_mutex_unlock(&global_lock);
            break;
        }

        printf("Thread ID: %lu, Letter: %c\n", pthread_self(), input_string[current_pos]);
        current_pos++;

        if (current_pos < strlen(input_string))
        {
            int next = char_to_index(input_string[current_pos]);
            if (next != -1)
            {
                pthread_mutex_lock(&sync[next].mutex);
                sync[next].ready = 1;
                pthread_cond_signal(&sync[next].cond);
                pthread_mutex_unlock(&sync[next].mutex);
            }
        }
        pthread_mutex_unlock(&global_lock);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    struct gengetopt_args_info args;
    if (cmdline_parser(argc, argv, &args) != 0)
        return EXIT_FAILURE;

    input_string = args.str_arg;

    for (size_t i = 0; i < strlen(input_string); i++)
    {
        if (!((input_string[i] >= 'a' && input_string[i] <= 'z') ||
              (input_string[i] >= 'A' && input_string[i] <= 'Z')))
        {
            ERROR(EXIT_FAILURE, "Invalid character in string");
        }
    }

    printf("String recebida: %s\n", input_string);

    pthread_t threads[26];

    for (int i = 0; i < 26; i++)
    {
        pthread_mutex_init(&sync[i].mutex, NULL);
        pthread_cond_init(&sync[i].cond, NULL);
        sync[i].ready = 0;

        int *index = malloc(sizeof(int));
        *index = i;
        pthread_create(&threads[i], NULL, thread_function, index);
    }

    int first = char_to_index(input_string[0]);
    pthread_mutex_lock(&sync[first].mutex);
    sync[first].ready = 1;
    pthread_cond_signal(&sync[first].cond);
    pthread_mutex_unlock(&sync[first].mutex);

    pthread_mutex_lock(&global_lock);
    while (current_pos < strlen(input_string))
    {
        pthread_mutex_unlock(&global_lock);
        pthread_mutex_lock(&global_lock);
    }
    pthread_mutex_unlock(&global_lock);

    for (int i = 0; i < 26; i++)
    {
        pthread_mutex_lock(&sync[i].mutex);
        sync[i].ready = 1;
        pthread_cond_signal(&sync[i].cond);
        pthread_mutex_unlock(&sync[i].mutex);
    }

    for (int i = 0; i < 26; i++)
    {
        pthread_join(threads[i], NULL);
        pthread_mutex_destroy(&sync[i].mutex);
        pthread_cond_destroy(&sync[i].cond);
    }

    printf("String escrita: %s\n", input_string);
    cmdline_parser_free(&args);
    return EXIT_SUCCESS;
}