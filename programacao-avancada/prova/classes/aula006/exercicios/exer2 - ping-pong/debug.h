/* <%LICENSE%> */
/**
 * @file debug.h
 * @brief Macros das fun��es de depura��o
 *
 * Macros que ser�o usadas nos programas desenvolvidos ao longo
 * do livro. Estas macros podem receber um n�mero vari�vel de par�metros
 * atrav�s de uma string de formata��o como no "printf". Seguem-se alguns
 * exemplos:
 * @code
 * DEBUG("i = %d e f=.2f%", i, f);
 * ERROR("%s", msg);
 * @endcode
 * @author Miguel Frade, Patricio Domingues, Vitor Carreira
 * @date Agosto de 2003
 * @version 2
 * @par Diferen�as da �ltima vers�o
 * Actualiza��o dos coment�rios doxygen
 * Inclus�o do "exitCode" nas macros ERROR e H_ERROR
 */
#ifndef DEBUG_H
#define DEBUG_H

void debug(const char *file, const int line, char *fmt, ...);
void warning(const char *file, const int line, char *fmt, ...);
void my_error(const char *file, const int line, int exitCode, char *fmt, ...);
void h_warning(const char *file, const int line, char *fmt, ...);
void h_error(const char *file, const int line, int exitCode, char *fmt, ...);

/**
 * Macro para imprimir no stderr informa��es �teis
 * para depura��o. O n�mero de par�metros de entrada
 * � vari�vel.
 *
 * @return A fun��o n�o retorna nada
 * @see debug()
 */
#define DEBUG(...) debug(__FILE__, __LINE__, __VA_ARGS__)

/**
 * Macro para imprimir no stderr informa��o relacionada
 * com insucesso de chamadas de fun��es, mas n�o termina a
 * execu��o do programa. O n�mero de par�metros de entrada
 * � vari�vel.
 *
 * @return A fun��o n�o retorna nada
 * @see warning()
 */
#define WARNING(...) warning(__FILE__, __LINE__, __VA_ARGS__)

/**
 * Macro para imprimir no stderr informa��o relacionada
 * com insucesso de chamadas de fun��es e termina a execu��o
 * do programa. O n�mero de par�metros de entrada � vari�vel.
 *
 * @return A fun��o n�o retorna nada
 * @see error()
 */
#define ERROR(exitCode, ...) my_error(__FILE__, __LINE__, (exitCode), __VA_ARGS__)

/**
 * Macro para imprimir no stderr informa��o relacionada
 * com insucesso de chamadas de fun��es de resolu��o de nomes,
 * mas n�o termina a execu��o do programa. O n�mero de
 * par�metros de entrada � vari�vel.
 *
 * @return A fun��o n�o retorna nada
 * @see h_warning()
 */
#define H_WARNING(...) h_warning(__FILE__, __LINE__, __VA_ARGS__)

/**
 * Macro para imprimir no stderr informa��o relacionada
 * com insucesso de chamadas de fun��es de resolu��o de nomes
 * e termina a execu��o do programa. O n�mero de par�metros
 * de entrada � vari�vel.
 *
 * @return A fun��o n�o retorna nada
 * @see h_error()
 */
#define H_ERROR(exitCode, ...) \
    h_error(__FILE__, __LINE__, (exitCode), __VA_ARGS__)

#endif /* DEBUG_H */
