
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char **cmd1;
char **cmd2;

int main(int argc, char *argv[])
{

    cmd1 = malloc(10*sizeof(char));
    cmd2 = malloc(10*sizeof(char));
    int j = 0;
    char *token = strtok(argv[1], " ");
    while(token != NULL) {
        if(strcmp(token, "|")==0) break;
        cmd1[j++] = token;
        token = strtok(NULL, " ");
    }
    cmd1[j] = NULL;
    token = strtok(NULL, " ");
    j = 0;
    while(token!=NULL) {
        cmd2[j++] = token;
        token = strtok(NULL, " ");
    }
    cmd2[j] = NULL;
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