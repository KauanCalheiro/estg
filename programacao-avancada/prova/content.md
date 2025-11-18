## 🧠 1. Conceitos Fundamentais de C

### Ponteiros

* Um **ponteiro** armazena o **endereço de memória** de outra variável.

```c
int x = 10;
int *p = &x;   // p aponta para x
printf("%d", *p); // imprime 10
```

* \* → desreferenciação (acesso ao valor apontado)
* & → operador de endereço (obtém o endereço de uma variável)

* `*p` → valor apontado
* `&x` → endereço de x

### Ponteiros e funções (passagem por referência)

```c
void soma(int *a, int *b) { *a += *b; }

int main() {
    int x=2, y=3;
    soma(&x, &y);
    printf("%d", x); // 5
}
```

### Alocação dinâmica

Usada quando o tamanho só é conhecido em tempo de execução.

```c
int *v = malloc(5 * sizeof(int));
if (!v) exit(1);
for (int i=0; i<5; i++) v[i] = i*i;
free(v);
```

### Structs

Permitem agrupar dados heterogéneos.

```c
typedef struct {
    int id;
    char nome[50];
    float nota;
} Aluno;
```

---

## 💾 2. Manipulação de Ficheiros

Muito usada nos labs com `fopen`, `fread`, `fwrite`, `fclose`:

```c
FILE *f = fopen("dados.txt", "w");
fprintf(f, "Olá Mundo!\n");
fclose(f);
```

Leitura:

```c
FILE *f = fopen("dados.txt", "r");
char linha[100];
while (fgets(linha, sizeof(linha), f)) printf("%s", linha);
fclose(f);
```

---

## 🔁 3. Processos e Execução (Ficha 1)

### `fork()`

Cria um novo processo (pai e filho).

```c
pid_t pid = fork();
if (pid == 0)
    printf("Filho: PID=%d\n", getpid());
else
    printf("Pai: PID=%d\n", getpid());
```

### `execvp()`

Substituem o processo atual ou executam comandos externos.

```c
execl("/bin/ls", "ls", "-a", "-l", NULL);
```

🧠 Internamente isso equivale a executar no terminal:

```bash
ls -a -l
```

Explicação dos parâmetros:

| Posição | Valor     | Significado                                                 |
| ------- | --------- | ----------------------------------------------------------- |
| `path`  | `/bin/ls` | Caminho do binário do comando `ls`                          |
| `arg0`  | `"ls"`    | Nome do programa (vai aparecer em `argv[0]` dentro do `ls`) |
| `arg1`  | `"-a"`    | Primeiro argumento do `ls`                                  |
| `arg2`  | `"-l"`    | Segundo argumento do `ls`                                   |
| `NULL`  | —         | Final da lista de argumentos                                |

---

## ⚡ 4. Sinais (Ficha 2)

Permitem reagir a eventos assíncronos.

### `sigaction`

```c
void handler(int sig){ printf("Sinal %d recebido!\n", sig); }

int main() {
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
    pause(); // espera sinal
}
```

### Enviar sinal

```bash
kill -s SIGUSR1 <pid>
```

### Alarmes

```c
alarm(5); // dispara SIGALRM em 5 segundos
```

---

## 🧵 5. Threads POSIX (Fichas 3 e 4)

### Criação de threads

```c
void *tarefa(void *arg){ printf("Thread %lu\n", pthread_self()); return NULL; }

int main(){
    pthread_t tid;
    pthread_create(&tid, NULL, tarefa, NULL);
    pthread_join(tid, NULL);
}
```

### Passagem de argumentos via `struct`

```c
typedef struct { int inicio, fim; } Dados;
void *processa(void *arg) {
    Dados *d = arg;
    printf("Thread processa [%d-%d]\n", d->inicio, d->fim);
    return NULL;
}
```

---

## 🔒 6. Sincronização: Mutex e Condição (Ficha 4)

### Mutex

Garante exclusão mútua.

```c
pthread_mutex_t mtx;
pthread_mutex_init(&mtx, NULL);
pthread_mutex_lock(&mtx);
  // secção crítica
pthread_mutex_unlock(&mtx);
pthread_mutex_destroy(&mtx);
```

### Variáveis de condição

Permitem esperar por um evento.

```c
pthread_cond_t cond;
pthread_cond_init(&cond, NULL);

pthread_mutex_lock(&mtx);
while(!condicao)
    pthread_cond_wait(&cond, &mtx);
pthread_mutex_unlock(&mtx);

pthread_cond_signal(&cond); // acorda 1 thread
pthread_cond_broadcast(&cond); // acorda todas
```

📘 **Exemplo clássico (Ping-Pong – igual ao teu código)**:

```c
while (*ping == 0) pthread_cond_wait(cond, mutex);
printf("ping...\n");
*ping = 0; *pong = 1;
pthread_cond_signal(cond);
```

---

## ✅ 8. Tópicos que mais caem na prova

| Tema                           | O que domina                                               |
| ------------------------------ | ---------------------------------------------------------- |
| **Threads e sincronização**    | `pthread_create`, `pthread_join`, `mutex`, `cond`          |
| **Comunicação interprocessos** | `fork`, `exec`, `pipe`, `wait`, `signal`                   |
| **Sinais e tratadores**        | `sigaction`, `pause`, `kill`, `alarm`                      |
| **Memória e ponteiros**        | `malloc`, `free`, `struct`, passagem por referência        |
| **UDP sockets**                | `sendto`, `recvfrom`, `inet_aton`, validação de IP e porta |
| **Gengetopt**                  | parâmetros `--size`, `--numthreads`, `--port`, etc.        |

---