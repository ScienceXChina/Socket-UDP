/*ex8-8.c*/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_PORT 6666
#define MAX_MSG 1024

void udpc_requ(int sockfd, const struct sockaddr_in *addr, int size)
{
    int n;
    char recvbuff[MAX_MSG], sndbuff[MAX_MSG];

    while(fgets(sndbuff,MAX_MSG,stdin) != NULL)
    {
        if(sendto(sockfd, sndbuff, sizeof(sndbuff), 0, (struct sockaddr *)addr, size) < 0)
        {
            printf("(client)sending error.\n");
            exit(1);
        }
        if((n=recvfrom(sockfd,recvbuff,MAX_MSG,0, (struct sockaddr *)addr, &size)) < 0)
        {
            printf("(client)receiving error.\n");
            exit(1);
        }
        recvbuff[n]=0;
        printf("Client received message: %s from IP->%s :%d \n", recvbuff, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
    }
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in  servaddr;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd < 0)
    {
        printf("Socket created failed.\n");
        return -1;
    }

    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // printf("IP->%s :%d \n", inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));

    udpc_requ(sockfd, &servaddr, sizeof(struct sockaddr_in));
    close(sockfd);
    return 0;
}
