/*ex8-7.c*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>

#define MAXMSG 1024

int main()
{
    int sockfd,nbytes;
    struct sockaddr_in addr;
    socklen_t size;
    char message[MAXMSG];

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd<0)
    {
        printf("Socket created failed.\n");
        return -1;
    }

    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(6666);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("bind failed.\n");
        return -1;
    }

    while(1)
    {
        size=sizeof(addr);
        nbytes=recvfrom(sockfd,message,MAXMSG,0,(struct sockaddr *)&addr,&size);
        if(nbytes<0)
        {
          printf("recvfrom(server) failed.\n");
          return -1;
        }
        printf("Server got message %s from IP->%s :%d \n", message, inet_ntoa(addr.sin_addr) ,htons(addr.sin_port));
        nbytes=sendto(sockfd,message,nbytes,0,(struct sockaddr*)&addr,size);
        if(nbytes<0)
        {
          printf("sendto(server) failed.\n");
          return -1;
        }
    }
}
