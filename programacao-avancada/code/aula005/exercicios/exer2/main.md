# Programação concorrente

## Concorrencia

apenas uma thread pode escrever em uma variavel compartilhada por vez

caso não seja "sincronizada" há a sobreescrita de valores

## RISK & CISK (???)

## Atomicidade (Programação)

A transação inteira é executada ou nenhuma parte dela é executada.

## Static em C


variavel dentro da funcao: só é inicializada uma vez
```c
void acrescenta(void) {
    static int contador = 0; // só é inicializado uma vez
    contador++;
    printf("%d\n", contador);
}

int main(void) {
    acrescenta(); // 1
    acrescenta(); // 2
    acrescenta(); // 3
    return 0;
}
```

static em funcao: só é visivel dentro do ficheiro
```c
static void funcao(void) {
    // só é visivel dentro deste ficheiro
}
```

## thread-safe

função que pode ser chamada por várias threads ao mesmo tempo sem causar problemas

ex: safe

```c
#include <stdio.h>
#include <pthread.h>

void safe_increment(int *value, pthread_mutex_t *lock) {
    pthread_mutex_lock(lock); // bloqueia o acesso
    (*value)++;               // incrementa o valor
    pthread_mutex_unlock(lock); // desbloqueia o acesso
}
```

ex: non-safe

```c
#include <stdio.h>
#include <pthread.h>

void unsafe_increment(int *value) {
    (*value)++; // incrementa o valor sem proteção
}
```

## mutex

mutual exclusion

e designado como um semáforo binário, onde possui 2 estados `Lock` e `Unlock`

## race condicion

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 15
#define NUM_INCREMENTS 20
#define INITIAL_VALUE 0

int shared_counter = INITIAL_VALUE;
pthread_mutex_t lock;
void* increment_counter(void* arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        pthread_mutex_lock(&lock); // Bloqueia o acesso ao contador compartilhado
        shared_counter++;           // Incrementa o contador compartilhado
        pthread_mutex_unlock(&lock); // Desbloqueia o acesso ao contador compartilhado
        usleep(rand() % 100);      // Simula algum trabalho
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL); // Inicializa o mutex

    // Cria as threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_counter, NULL);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", shared_counter); // Deve ser NUM_THREADS * NUM_INCREMENTS

    pthread_mutex_destroy(&lock); // Destroi o mutex
    return 0;
}
```
/// E se usasse um pool que encapsula essa logica?

