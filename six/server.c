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
    char* quotes[5] = {"In the bleak mid winter...", "Shiver me timbers!", "Never one without the other.", "I will hack and chop and cleave and sunder the filth of your forms!", "nyahh"};
    int sock;
   // socklen_t length;
    struct sockaddr_in name, client;
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
    while(1) {
        char* buf = malloc(1024*sizeof(char));
        if (recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client, &remoteClientSize) < 0)
            perror("server receiving datagram packet");
        if (prefix("GET", buf)) {
            strsep(&buf, ":");
            int n = atoi(buf);
            for(int i = 0; i < n; i++) {
                strcpy(buf, "QUOTE:");
                int r = rand()%N;
                strcat(buf, quotes[r]);
                if (sendto(sock, buf, strlen(buf)+1, 0, (struct sockaddr *)&client, remoteClientSize)<0)
                    perror("sending back quote.");
                *buf = '\0';
            }
       }
        else printf("received:\n%s\n", buf);
    }
    
    return EXIT_SUCCESS;
}