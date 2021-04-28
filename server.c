#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE sizeof(struct sockaddr_in)

int main()
{
    int sockfd;

    struct sockaddr_in server = {SIZE, AF_INET, 7000, INADDR_ANY};

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("sock: l15");
        exit(1);
    }

    printf("Binding...\n");

    if (bind(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1)
    {
        perror("sock: bind: l20");
        exit(1);
    }
    printf("listening....\n");

    if ((listen(sockfd, 5)) == -1)
    {
        perror("sock: listen: l27");
        exit(1);
    }

    int newsockfd;
    while (1)
    {
        printf("Accepting....\n");
        if ( (newsockfd = accept(sockfd, 0, 0)) == -1 )
        {
            perror("sock: accept: l35");
            exit(1);
        }
        printf("Accepted %i\n", sockfd);

        char * str = "Hello\n";

        //
        //read
        write(newsockfd, str, strlen(str));

        close(newsockfd);
        //
    }

    return 0;
}