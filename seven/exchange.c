#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static int count;
static void handler_parent()
{
    printf("%d: Parent received signal\n", getpid());
    printf("count increased: %d\n", ++count);
}
static void handler_child()
{
    printf("%d: Child received signal\n", getpid());
}

int main(int argc, char *argv[])
{
    count = 0;
    pid_t pid;
    if (signal(SIGUSR1, handler_parent) == SIG_ERR)
    { /* signal error */
        fprintf(stderr, "Can't catch SIGUSR1: %s\n", strerror(errno));
    }
    if (signal(SIGUSR2, handler_child) == SIG_ERR)
    { /* signal error */
        fprintf(stderr, "Can't catch SIGUSR2: %s\n", strerror(errno));
    }
    if ((pid = fork()) < 0)
    { /* fork error */
        perror("fork error.");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        /* parent’s code */
        kill(pid, SIGUSR2);
        for(;;)
            pause();
    }
    else
    {
        /* child’s code */
        for(int i = 0; i < 3; i++){
            kill(getppid(), SIGUSR1);
            sleep(1);
        }
        kill(getppid(), SIGKILL);
    }
}
