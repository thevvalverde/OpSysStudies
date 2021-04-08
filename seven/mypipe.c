
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *cmd1[] = {"ls", "-l", NULL};
char *cmd2[] = {"wc", "-l", NULL};

int main(int argc, char *argv[])
{
    int fd[2];
    pid_t pid;
    if (pipe(fd) < 0)
    {
        /* pipe error */
        perror("Pipe error.");
        exit(EXIT_FAILURE);
    }
    if ((pid = fork()) < 0)
    {
        /* fork error */
        perror("Fork error.");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO); // stdout to pipe
        close(fd[1]);
        // parent writes to the pipe
        if (execvp(cmd1[0], cmd1) < 0)
        {
            /* exec error */
            perror("Exec error.");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO); // stdin from pipe
        close(fd[0]);
        if (execvp(cmd2[0], cmd2) < 0)
        {
            /* exec error */
            perror("Exec error.");
            exit(EXIT_FAILURE);
        }
    }
}