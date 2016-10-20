/*ex8-7.c*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 6666
#define MAX_MSG 1024

void udps_respon(int sockfd)
{
    struct sockaddr_in addr;
    int nbytes;
    socklen_t size;
    char message[MAX_MSG];

    while(1)
    {
        size = sizeof(struct sockaddr_in);
        nbytes = recvfrom(sockfd,message,MAX_MSG,0,(struct sockaddr *)&addr,&size);
        if(nbytes < 0)
        {
          printf("recvfrom(server) failed.\n");
          exit(1);
        }
        printf("Server got message %s from IP->%s :%d \n", message, inet_ntoa(addr.sin_addr) ,ntohs(addr.sin_port));
        nbytes = sendto(sockfd,message,nbytes,0,(struct sockaddr*)&addr,size);
        if(nbytes < 0)
        {
          printf("sendto(server) failed.\n");
          exit(1);
        }
    }
}

int main(void)
{
    int sockfd;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd < 0)
    {
        printf("Socket created failed.\n");
        return -1;
    }

    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        printf("bind failed.\n");
        return -1;
    }

    printf("IP->%s :%d \n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    udps_respon(sockfd);
    close(sockfd);
}
