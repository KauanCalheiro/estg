# 🧩 **Utilitários Essenciais em C**

---

## 🧠 1. Manipulação de Strings

### 🔡 Converter para minúsculas / maiúsculas

```c
#include <ctype.h>

void str_to_lower(char *s) {
    for (int i = 0; s[i]; i++)
        s[i] = tolower(s[i]);
}

void str_to_upper(char *s) {
    for (int i = 0; s[i]; i++)
        s[i] = toupper(s[i]);
}
```

---

### 🔠 Verificar se todos os caracteres são letras

```c
#include <ctype.h>

int is_all_letters(const char *s) {
    for (int i = 0; s[i]; i++)
        if (!isalpha(s[i]))
            return 0;
    return 1;
}
```

---

### 🔢 Verificar se todos os caracteres são dígitos

```c
#include <ctype.h>
#include <string.h>

int is_all_digits(const char *s) {
    for (int i = 0; i < strlen(s); i++)
        if (!isdigit(s[i]))
            return 0;
    return 1;
}
```

---

### 🧮 Verificar se são todas maiúsculas ou minúsculas

```c
#include <ctype.h>

int is_all_upper(const char *s) {
    for (int i = 0; s[i]; i++)
        if (!isupper(s[i]))
            return 0;
    return 1;
}

int is_all_lower(const char *s) {
    for (int i = 0; s[i]; i++)
        if (!islower(s[i]))
            return 0;
    return 1;
}
```

---

### ✂️ Remover newline (`\n`)

```c
#include <string.h>

void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}
```

---

### ␣ Verificar se string contém apenas espaços

```c
#include <ctype.h>

int is_blank(const char *s) {
    for (int i = 0; s[i]; i++)
        if (!isspace(s[i]))
            return 0;
    return 1;
}
```

---

### 🔍 Procurar substring dentro de string

```c
#include <string.h>

int str_contains(const char *str, const char *sub) {
    return strstr(str, sub) != NULL;
}
```

---

### 🔁 Inverter string

```c
#include <string.h>

void reverse_string(char *s) {
    int n = strlen(s);
    for (int i = 0; i < n / 2; i++) {
        char tmp = s[i];
        s[i] = s[n - i - 1];
        s[n - i - 1] = tmp;
    }
}
```

---

## 🔢 2. Conversão Segura de Strings

```c
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int safe_atoi(const char *s, int *result) {
    char *endptr;
    errno = 0;
    long val = strtol(s, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || val > INT_MAX || val < INT_MIN)
        return 0;
    *result = (int)val;
    return 1;
}
```

---

## 🧮 3. Manipulação de Arrays

```c
void fill_array(int *arr, int size, int value) {
    for (int i = 0; i < size; i++)
        arr[i] = value;
}

void swap_int(int *a, int *b) {
    int temp = *a; *a = *b; *b = temp;
}

#include <string.h>
void copy_array(int *dest, const int *src, int size) {
    memcpy(dest, src, size * sizeof(int));
}

#include <string.h>
int compare_arrays(const int *a, const int *b, int size) {
    return memcmp(a, b, size * sizeof(int)) == 0;
}
```

---

## 💾 4. Manipulação de Memória

```c
#include <stdlib.h>

int *alloc_int_array(int size, int init_value) {
    int *arr = malloc(size * sizeof(int));
    if (!arr) return NULL;
    for (int i = 0; i < size; i++)
        arr[i] = init_value;
    return arr;
}

void free_and_null(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}
```

---

## 📂 5. Manipulação de Ficheiros

### Verificar se o ficheiro existe

```c
#include <stdio.h>

int file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    fclose(f);
    return 1;
}
```

---

### Ler primeira linha

```c
#include <stdio.h>

int file_read_first_line(const char *path, char *buffer, size_t size) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    if (!fgets(buffer, size, f)) { fclose(f); return 0; }
    fclose(f);
    return 1;
}
```

---

### Contar número de linhas

```c
#include <stdio.h>

int file_count_lines(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int count = 0, ch;
    while ((ch = fgetc(f)) != EOF)
        if (ch == '\n') count++;
    fclose(f);
    return count;
}
```

---

### Copiar ficheiro

```c
#include <stdio.h>

int file_copy(const char *src, const char *dst) {
    FILE *in = fopen(src, "r");
    FILE *out = fopen(dst, "w");
    if (!in || !out) return 0;

    char buf[1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
        fwrite(buf, 1, n, out);

    fclose(in);
    fclose(out);
    return 1;
}
```

---

### Anexar linha

```c
#include <stdio.h>

int file_append_line(const char *path, const char *line) {
    FILE *f = fopen(path, "a");
    if (!f) return 0;
    fprintf(f, "%s\n", line);
    fclose(f);
    return 1;
}
```

---

### 📖 Ler ficheiro inteiro (carregar todo conteúdo em memória)

```c
#include <stdio.h>
#include <stdlib.h>

char *file_read_all(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buffer = malloc(size + 1);
    if (!buffer) { fclose(f); return NULL; }

    fread(buffer, 1, size, f);
    buffer[size] = '\0';

    fclose(f);
    return buffer; // lembre-se de usar free() depois
}
```

📘 *Excelente para carregar ficheiros de configuração ou texto completo.*

---

## ⚙️ 6. Utilitários Gerais

```c
int in_range(int val, int min, int max) {
    return val >= min && val <= max;
}

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
int rand_between(int min, int max) {
    unsigned int seed = time(NULL) ^ getpid();
    return min + rand_r(&seed) % (max - min + 1);
}
```

---

## 🖨️ 7. Especificadores do `printf`

| Tipo de dado                           | Formato                   | Exemplo de saída                      |
| -------------------------------------- | ------------------------- | ------------------------------------- |
| **Inteiros**                           | `%d` / `%i`               | `printf("%d", 42); → 42`              |
| **Sem sinal**                          | `%u`                      | `printf("%u", 42); → 42`              |
| **Octal**                              | `%o`                      | `printf("%o", 10); → 12`              |
| **Hexadecimal (minúsculo)**            | `%x`                      | `printf("%x", 255); → ff`             |
| **Hexadecimal (maiúsculo)**            | `%X`                      | `printf("%X", 255); → FF`             |
| **Caractere**                          | `%c`                      | `printf("%c", 'A'); → A`              |
| **String**                             | `%s`                      | `printf("%s", "Olá"); → Olá`          |
| **Endereço (ponteiro)**                | `%p`                      | `printf("%p", ptr); → 0x7ffeeabc1234` |
| **Float / double**                     | `%f`                      | `printf("%f", 3.14); → 3.140000`      |
| **Notação científica**                 | `%e` / `%E`               | `printf("%e", 3.14); → 3.140000e+00`  |
| **Formato geral (float/adapta)**       | `%g` / `%G`               | `printf("%g", 3.14); → 3.14`          |
| **Escrever número de bytes impressos** | `%n`                      | `printf("%d%n", 10, &count);`         |
| **Sinalizar erro (printf stderr)**     | `fprintf(stderr, "...");` | Envia saída para erro padrão          |
| **Caractere de escape**                | `%%`                      | `printf("100%%"); → 100%`             |

🔧 **Modificadores úteis:**

| Modificador | Efeito                               |
| ----------- | ------------------------------------ |
| `%5d`       | Alinha número à direita em 5 espaços |
| `%-5d`      | Alinha número à esquerda             |
| `%05d`      | Preenche com zeros à esquerda        |
| `%.2f`      | Define 2 casas decimais              |
| `%10s`      | Largura mínima de 10 caracteres      |
| `%.*s`      | Define tamanho dinâmico da string    |

---

## ✅ Resumo Final

| Categoria     | Funções principais                                                                                                              |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------- |
| **Strings**   | `str_to_lower`, `str_to_upper`, `trim_newline`, `is_all_digits`, `is_all_letters`, `str_contains`, `reverse_string`, `is_blank` |
| **Conversão** | `safe_atoi`                                                                                                                     |
| **Arrays**    | `fill_array`, `swap_int`, `copy_array`, `compare_arrays`                                                                        |
| **Memória**   | `alloc_int_array`, `free_and_null`                                                                                              |
| **Ficheiros** | `file_exists`, `file_copy`, `file_read_all`, `file_read_first_line`, `file_count_lines`, `file_append_line`                     |
| **Geral**     | `in_range`, `rand_between`                                                                                                      |
| **Output**    | Especificadores de `printf` e modificadores de formato                                                                          |
