#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

#define SLEEP_TIME 1
#define FORK_ERROR -1
#define FORK_CHILD_PROCESS 0
#define KILL_ALL_RELATED 0

char *file_arg = NULL;

void handle_signal(int signum, siginfo_t *info, void *context);
void read_first_line(const char *filename);
void parent_process();
void child_process();

void handle_signal(int signum, siginfo_t *info, void *context)
{
    int aux = errno; // salvar errno
    (void)context;

    if (signum == SIGINT)
    {
        printf("\n[Parent] SIGINT recebido. Terminando...\n");
        kill(KILL_ALL_RELATED, SIGTERM);
        exit(EXIT_SUCCESS);
    }
    else if (signum == SIGUSR1)
    {
        printf("\n[Parent] SIGUSR1 recebido do filho %d\n", info->si_pid);
        read_first_line(file_arg);
    }

    errno = aux; // restaurar errno
}

void read_first_line(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char *line = NULL;
    size_t len = 0;

    if (getline(&line, &len, file) != -1)
    {
        printf("First line: %s\n", line);
    }
    else
    {
        printf("File is empty or error reading file\n");
    }

    fclose(file);
}

void parent_process()
{
    printf("Parent PID: %d\n", getpid());
    while (1)
    {
        pause(); // espera sinais
    }
}

void child_process()
{
    printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
    while (1)
    {
        sleep(SLEEP_TIME);
        if (kill(getppid(), SIGUSR1) != 0)
        {
            perror("kill");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[])
{
    /* Initialize argument structure */
    struct gengetopt_args_info args;

    if (cmdline_parser(argc, argv, &args) != 0)
    {
        return 1;
    }

    /* Setup signal handler */
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction = handle_signal;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction(SIGINT)");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction(SIGUSR1)");
        exit(EXIT_FAILURE);
    }

    /* Get arguments */
    file_arg = args.file_arg;

    printf("File argument: %s\n", file_arg);
    printf("Main process PID: %d\n\n", getpid());

    pid_t pid = fork();

    switch (pid)
    {
    case FORK_ERROR:
        perror("fork failed");
        exit(EXIT_FAILURE);
        break;
    case FORK_CHILD_PROCESS:
        child_process();
        break;
    default:
        parent_process();
        break;
    }

    /* Cleanup */
    cmdline_parser_free(&args);
    return 0;
}
