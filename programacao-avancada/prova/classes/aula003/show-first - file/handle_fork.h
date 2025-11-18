#ifndef HANDLE_FORK_H
#define HANDLE_FORK_H

#include <unistd.h>

/* Tipo de callback genérico que recebe um ponteiro de argumento */
typedef void (*proc_func)(void *arg);

/**
 * @brief Função genérica para lidar com fork
 * 
 * @param pid        pid retornado pelo fork()
 * @param parent_cb  função callback do processo pai
 * @param parent_arg argumento para a função pai (pode ser NULL)
 * @param child_cb   função callback do processo filho
 * @param child_arg  argumento para a função filho (pode ser NULL)
 */
void handle_fork(pid_t pid, proc_func parent_cb, void *parent_arg,
                 proc_func child_cb, void *child_arg);

#endif
