#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define N 5;
#define LOCALHOST "localhost"

bool prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int sock;
   // socklen_t length;
    struct sockaddr_in name, client;
    char *list[10];  
    /* create socket from which to read */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("opening datagram socket");
        return EXIT_FAILURE;
    }
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(12000);
    if (bind(sock, (struct sockaddr *)&name, sizeof(name)))
    {
        perror("server binding datagram socket");
        return EXIT_FAILURE;
    }
    /* find assigned port value and print it out */
    /*length = (socklen_t)sizeof(name);
    if (getsockname(sock, (struct sockaddr *)&name, &length))
    {
        perror("getting socket name");
        return EXIT_FAILURE;
    }
    printf("Socket has port #%d\n", ntohs(name.sin_port));*/
    /* read from the socket */
    listen(sock, 3); 
    socklen_t remoteClientSize = sizeof(client);
    int counter = 0;
    while(1) {
        char* buf = malloc(1024*sizeof(char));
        if (recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&client, &remoteClientSize) < 0)
            perror("server receiving datagram packet");

        printf("Received a message %s\n", buf);

        if (prefix("PUT", buf)) {
            //printf("buffer: %s\n", buf);
            strsep(&buf, ":");
            char* username = strsep(&buf, ":");
            char* message = strsep(&buf, ":");
            //printf("username: %s     message: %s\n", username, message);
            if(counter==0) list[counter++] = username;
            list[counter++] = message;
            list[counter] = '\0';
        }else {
            //strtok(buf, ":");
            printf("%d\n", counter);
            char* incoming = malloc(255*sizeof(char));
            sprintf(incoming, "INCOMING:%d", counter-1);
            //printf("incoming: %s", incoming);
            if (sendto(sock, incoming, strlen(incoming), 0, (struct sockaddr *)&client, remoteClientSize)<0)
                perror("sending back quote.");

            for(int i = 1; i < counter; i++) {
               // printf("list[%d]: %s\n", i, list[i]);
                strcpy(buf, list[i]);
                if (sendto(sock, buf, strlen(buf)+1, 0, (struct sockaddr *)&client, remoteClientSize)<0)
                    perror("sending back quote.");
            }
            memset(list, 0, sizeof(list));
      }
    }
    
    return EXIT_SUCCESS;
}