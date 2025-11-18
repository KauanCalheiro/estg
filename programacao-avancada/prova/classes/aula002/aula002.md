# diferenca entre programa e processo

# fork | wait
#include <sys/wait.h>

Tem que ser carregado na memoria para ser executado.

podemos ter varios processos do mesmo programa rodando ao mesmo tempo.

fork -> cria um novo processo (processo filho) a partir do processo atual (processo pai)
wait -> espera o processo filho terminar para continuar a execucao do processo pai


# exec
execp -> qunado ja sabemos os paramentros que vao ser passados (funciona como um default)
execv -> quando nao sabemos os parametros que vao ser passados (definidos em argv pelo usuario)


## args.ggo

option "long-name-of-option" short_name_of_option "Description of option" type_of_option [typestr="type_string"] [default=value] [required|optional]


##

int i; //ocupa 4 bytes
int *ponteiro; //depende da arquitetura (32 ou 64 bits) -> se for 32 bits, ocupa 4 bytes, se for 64 bits, ocupa 8 bytes
char *ponteiro_char; //depende da arquitetura (32 ou 64 bits) -> se for 32 bits, ocupa 4 bytes, se for 64 bits, ocupa 8 bytes

``` c
int i = 10;
int *ponteiro = &i; //& -> operador de endereco

//altera valor de i
*ponteiro = 20; //* -> operador de desreferencia

//altera valor do ponteiro
ponteiro = 5; //altera o endereco do ponteiro

char c = 'A';
char *ponteiro_char = &c; //& -> operador de endereco

//altera valor de c
*ponteiro_char = 'B'; //* -> operador de desreferencia

//altera valor do ponteiro
ponteiro_char = 'C'; //altera o endereco do ponteiro
```

##

```C
int func(void){
    return 0;
}

int main(void){
    int (*ponteiro_func)(void); //declara um ponteiro para funcao que retorna int e nao recebe parametros
    ponteiro_func = func; //atribui o endereco da funcao ao ponteiro
    int resultado = ponteiro_func(); //chama a funcao atraves do ponteiro
    return 0;
}
```

```C
int func(int a, int b){
    return a + b;
}

int main(void){
    int (*ponteiro_func)(int, int); //declara um ponteiro para funcao que retorna int e recebe dois parametros inteiros
    ponteiro_func = func; //atribui o endereco da funcao ao ponteiro
    int resultado = ponteiro_func(10, 20); //chama a funcao atraves do ponteiro
    return 0;
}
```

```C
typedef int (*func_ptr_t)(int, int); //define um tipo para o ponteiro de funcao

func_ptr_t ponteiro_func; //declara um ponteiro para funcao que retorna int e recebe dois parametros inteiros
ponteiro_func = func; //atribui o endereco da funcao ao ponteiro
int resultado = ponteiro_func(10, 20); //chama a funcao atraves do ponteiro
```

Pode ser usado para callbacks