#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define DATA "Such is the nature of evil. \
Out there in the vast ignorance of the world it \
festers and spreads. A shadow that grows in the dark. \
A sleepless malice as black as the oncoming wall of night. \
So it ever was. So will it always be. \
In time all foul things come forth."
#define GETREQUEST "GET:"
#define PUTREQUEST "PUT:"
#define LOCALHOST "localhost"
#define SIZE 255

bool prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}
/*
* this is the client side
*/
int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in name;
    struct hostent *hp;
    char* request = malloc(SIZE*sizeof(char));
    char* username = argv[2];
    bool getbool = false;

    if(strcmp(argv[1], "GET")==0) getbool = true;
    if(getbool) strcat(request, GETREQUEST);
    else strcat(request, PUTREQUEST);
    strcat(request, username);
    if(!getbool)  {
        char* message = argv[3];
        strcat(request, ":");
        strcat(request, message);
    }
    printf("request: %s\n", request);
    /* create socket on which to send */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("opening datagram socket");
        return EXIT_FAILURE;
    }
    /* destination is constructed from localhost */
    if ((hp = gethostbyname(LOCALHOST)) == 0)
    {
        fprintf(stderr, "unknown host\n" );
        return EXIT_FAILURE;
    }
    memcpy(&name.sin_addr, hp->h_addr, hp->h_length);
    name.sin_family = AF_INET;
    name.sin_port = htons(12000);
    /* connect to given port */
    if (connect(sock, (struct sockaddr *)&name, sizeof(name)) < 0)
    {
        perror("client connecting to server socket");
        return EXIT_FAILURE;
    }
    /* send message */

    if (write(sock, request, SIZE) < 0)
        perror("client sending datagram message");
    if(prefix("PUT", request)) exit(EXIT_SUCCESS);

    char* buf = malloc(SIZE*sizeof(char));
    int n;
    if(read(sock, buf, SIZE)<0)
        perror("client receiving message.");
    if(prefix("INCOMING", buf)) {
        strsep(&buf, ":");
        n = atoi(buf);
        printf("Receiving %d messages:\n", n);
        for(int i = 0; i < n; i++) {
            char* num = malloc(50*sizeof(char));
            if(read(sock, num, SIZE)<0)
                perror("Reading message");
            printf("%s\n", num);
        }
        return EXIT_SUCCESS;
    }


    close(sock);
    return EXIT_SUCCESS;
}
