#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 50

int main(int argc, char *argv[]) {

    FILE *file;
    char *line = NULL;
    char *needle = strdup(argv[1]);
    size_t len = 0;
    ssize_t reader;
    int count = 1;
    int pos = 0;

    if (argv[1] == NULL || argv[2] == NULL || argc != 3) {
        printf("Usage: ./mygrep word file\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(argv[2], "r");
    if (file == NULL)
        exit(EXIT_FAILURE);
    
    while ((reader = getline(&line, &len, file)) != -1) {
        char* ptr = line;
        if((ptr =strstr(ptr, needle))) {
            pos = strlen(line) - strlen(ptr) + 1;
            printf("[%d:%d", count, pos);
            ptr++;
            while ((ptr = strstr(ptr, needle))) {
                pos = strlen(line) - strlen(ptr) + 1;
                printf(",%d", pos);
                ptr++;
            }
            printf("]\n");
        }
        count++;
    }
}