#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "debug.h"

#define C_ERROR_PTHREAD_CREATE 1

void *hello(void *arg);

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    pthread_t tid;

    printf("Main thread = [%lu]\n", (unsigned long)pthread_self());

    if ((errno = pthread_create(&tid, NULL, hello, NULL)) != 0) {
        ERROR(C_ERROR_PTHREAD_CREATE, "pthread_create() failed!");
    }

    void *result;
    if((errno = pthread_join(tid, &result)) != 0) {
        ERROR(1, "pthread_join() failed!");
    }

    int *int_result = (int *)result;
    printf("Thread returned: %d\n", *int_result);
    free(int_result);

    return EXIT_SUCCESS;
}

void *hello(void *arg) {
    (void)arg;

    printf("My name is Thread, Posix Thread = [%lu]\n", (unsigned long)pthread_self());

    int *i = malloc(sizeof(int));
    *i = 42;

    return i;
    // pthread_exit(NULL); encerra automaticamente a thread com o valor indicado
}
