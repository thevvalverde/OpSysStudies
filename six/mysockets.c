#include <sys/wait.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define SOCK0 0
#define SOCK1 1
#define DATA0 "In every walk with nature..."
#define DATA1 "...one receives far more than he seeks."
#define SIZE 1024
/* by John Muir */
int main(int argc, char *argv[])
{
    int sockets[2];
    char* buf = malloc(SIZE*sizeof(char));
    pid_t pid;
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0)
    {
        perror("opening stream socket pair");
        exit(1);
    }
    if ((pid = fork()) < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        /* this is the child */
        close(sockets[SOCK0]);
        if (read(sockets[SOCK1], buf, SIZE) < 0)
            perror("reading stream message");
        for(int i = 0; buf[i]; i++){
            buf[i] = tolower(buf[i]);
        }
        if (write(sockets[SOCK1], buf, SIZE) < 0)
            perror("writing stream message");
        close(sockets[SOCK1]);
        /* leave gracefully */
        return EXIT_SUCCESS;
    }
    else
    {
        /* this is the parent */
        FILE* fp = fopen(argv[1], "r");
        fseek(fp, 0, SEEK_END);
        size_t size = ftell(fp);
        rewind(fp);
        char* line = malloc(size*sizeof(char));
        getdelim(&line, &size, '\0', fp);
        close(sockets[SOCK1]);
        if (write(sockets[SOCK0], line, size) < 0)
            perror("writing stream message");
        if (read(sockets[SOCK0], buf, size) < 0)
            perror("reading stream message");
        printf("-->%s\n", buf);
        close(sockets[SOCK0]);
        /* wait for child and exit */
        if (waitpid(pid, NULL, 0) < 0)
        {
            perror("did not catch child exiting");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
}
