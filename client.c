#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>

#define SIZE sizeof(struct sockaddr_in)

int main()
{
    int sockfd;

    struct sockaddr_in server = {SIZE, AF_INET, 7000};
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //IP


    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("sock: : l16");
        exit(1);
    }

    printf("Connecting...\n");

    if ( connect(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr_in))==-1)
    {
        perror("sock: : l22");
        exit(1);
    }
    printf("Connected:%i\n", sockfd);

    char buf[1000];
    int res = 0;
    res = read(sockfd, buf, 20);
    printf("Readed %i bytes\n", res);
    buf[res] = 0;
    printf("Readed %s\n", buf);
    close(sockfd);

    return 0;
}