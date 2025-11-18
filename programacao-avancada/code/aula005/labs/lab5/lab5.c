#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/time.h>
#include "debug.h"

#define C_ERROR_PTHREAD_CREATE 1
#define C_ERROR_PTHREAD_JOIN 2
#define MAX 5

void *fatorial(void *arg);

struct ThreadParams {
    unsigned long valor;
    unsigned long resultado;
};

int main(void) {
    struct ThreadParams threadParams[MAX];

    // inicializar números para cálculo do fatorial (de 1 a 5)
    // "configurar" os parâmetros a passar às threads
    for (int i = 0; i < MAX; i++) {
        threadParams[i].valor = i + 1;
    }

    pthread_t tids[MAX];

    for (int i = 0; i < MAX; i++) {
        if ((errno = pthread_create(&tids[i], NULL, fatorial,
                                    &threadParams[i])) != 0) {
            ERROR(C_ERROR_PTHREAD_CREATE, "pthread_create() failed!");
        }
    }

    // esperar que todas as threads terminem para mostrar resultados
    for (int i = 0; i < MAX; i++) {
        if ((errno = pthread_join(tids[i], NULL)) != 0) {
            ERROR(C_ERROR_PTHREAD_JOIN, "pthread_join() failed!\n");
        }
    }

    printf("Resultados:\n");
    for (int i = 0; i < MAX; i++) {
        printf("\t%lu!: %lu\n", threadParams[i].valor,
               threadParams[i].resultado);
    }

    exit(0);
}

void *fatorial(void *arg) {
    // cast para o tipo de dados original
    struct ThreadParams *params = (struct ThreadParams *)arg;

    // obter valor para cálculo do fatorial
    unsigned long valor = params->valor;

    // calcular fatorial
    unsigned long aux = valor;
    while (--valor > 1) {
        aux *= valor;
    }

    // guardar valor calculado no vetor de resultados
    params->resultado = aux;

    // esperar um pouco (apenas para pergunta do próximo Lab)
    sleep(params->valor);

    return NULL;
}
