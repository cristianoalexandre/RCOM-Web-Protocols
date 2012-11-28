#include "clientTCP.h"

int connect_socket(char * hostip, int port)
{
    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char*) &server_addr, sizeof (server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(hostip); /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port); /*server TCP port must be network byte ordered */

    /*open an TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket()");
        exit(0);
    }

    printf("File descriptor created!\n");

    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof (server_addr)) < 0)
    {
        perror("connect()");
        exit(0);
    }

    printf("Connected!\n");
    printf("sockfd: %d\n", sockfd);

    return sockfd;
}


