/* <%LICENSE%> */
/**
 * @file debug.c
 * @brief Fun��es de depura��o
 *
 * Fun��es de depura��o que ser�o chamadas atrav�s das respectivas
 * macros definidas no ficheiro debug.h. O objectivo destas fun��es
 * � auxiliar o tratamento de erros e a depura��o
 *
 * @todo revis�o do c�digo
 *
 * @author Miguel Frade, Patricio Domingues, Vitor Carreira
 * @date Agosto de 2003
 * @version 2
 * @par Diferen�as da �ltima vers�o
 * Actualiza��o dos coment�rios doxygen.
 * As fun��es error e h_error passaram a incluir "exitCode"
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>

#include "debug.h"

/**
 * Esta fun��o deve ser utilizada para auxiliar a depura��o de programas.
 * Esta fun��o <b>n�o deve</b> ser chamada directamente, mas sim atrav�s
 * da macro DEBUG().
 *
 * @param file nome do ficheiro
 * 	       (atrav�s da macro DEBUG)
 * @param line linha onde a fun��o foi chamada
 * 	       (atrav�s da macro DEBUG)
 * @param fmt string de formata��o como no "printf"
 * @param ... n� vari�vel de par�metros
 * @return A fun��o n�o retorna nada
 * @see DEBUG
 */
void debug(const char *file, const int line, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] DEBUG - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    fflush(stderr);
}

/**
 * Fun��o que envia para o canal de erros a mensagem "WARNING" rotulada
 * com o nome do ficheiro e da linha da fun��o chamante e ainda da mensagem de
 * erro do sistema. A fun��o <b>n�o deve</b> ser chamada directamente, mas sim
 * atrav�s da macro WARNING().
 *
 * @param file nome do ficheiro fonte da fun��o chamante
 * 	       (atrav�s da macro WARNING)
 * @param line linha onde a fun��o foi chamada
 * 	       (atrav�s da macro WARNING)
 * @param fmt string de formata��o como no "printf"
 * @param ... n� vari�vel de par�metros
 * @return A fun��o n�o retorna nada
 * @see WARNING
 */
void warning(const char *file, const int line, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] WARNING - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, ": %s\n", strerror(errno));
    fflush(stderr);
}

/**
 * Fun��o que envia para o canal de erros a mensagem "ERROR" rotulada
 * com o nome do ficheiro e da linha da fun��o chamante, e ainda da mensagem de
 * erro do sistema. A fun��o <b>n�o deve</b> ser chamada directamente, mas sim
 * atrav�s da macro ERROR().
 *
 * @param file nome do ficheiro fonte da fun��o chamante
 * 	       (atrav�s da macro ERROR)
 * @param line linha onde a fun��o foi chamada
 * 	       (atrav�s da macro ERROR)
 * @param exitCode valor passado � fun��o "exit()"
 * @param fmt string de formata��o como no "printf"
 * @param ... n� vari�vel de par�metros
 * @return A fun��o n�o retorna nada
 * @see ERROR
 */
void my_error(const char *file, const int line, int exitCode, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] ERROR - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, ": %s\n", strerror(errno));
    fflush(stderr);
    exit(exitCode);
}

/**
 * Fun��o que envia para o canal de erros a mensagem "H_WARNING" rotulada
 * com o nome do ficheiro e da linha da fun��o chamante, e ainda da mensagem de
 * erro do sistema. A fun��o <b>n�o deve</b> ser chamada directamente, mas sim
 * atrav�s da macro H_WARNING().
 *
 * @param file nome do ficheiro fonte da fun��o chamante
 * 	       (atrav�s da macro H_WARNING)
 * @param line linha onde a fun��o foi chamada
 * 	       (atrav�s da macro H_WARNING)
 * @param fmt string de formata��o como no "printf"
 * @param ... n� vari�vel de par�metros
 * @return A fun��o n�o retorna nada
 * @see H_WARNING
 */
void h_warning(const char *file, const int line, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] H_WARNING - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, ": %s\n", hstrerror(h_errno));
    fflush(stderr);
}

/**
 * Fun��o que envia para o canal de erros a mensagem "H_ERROR" rotulada
 * com o nome do ficheiro e da linha da fun��o chamante, e ainda da mensagem de
 * erro do sistema. A fun��o <b>n�o deve</b> ser chamada directamente, mas sim
 * atrav�s da macro H_ERROR().
 *
 * @param file nome do ficheiro fonte da fun��o chamante
 * 	       (atrav�s da macro H_ERROR)
 * @param line linha onde a fun��o foi chamada
 * 	       (atrav�s da macro H_ERROR)
 * @param exitCode valor passado � fun��o "exit()"
 * @param fmt string de formata��o como no "printf"
 * @param ... n� vari�vel de par�metros
 * @return A fun��o n�o retorna nada
 * @see H_ERROR
 */
void h_error(const char *file, const int line, int exitCode, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] H_ERROR - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, ": %s\n", hstrerror(h_errno));
    fflush(stderr);
    exit(exitCode);
}
