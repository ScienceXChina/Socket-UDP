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

char *os = {"Ubuntu"};
unsigned int CS_counter;

#pragma pack(1)
typedef struct DATA{
  int nodeSize;
  char os_name[20];
  unsigned int num;
}UDP_data;
#pragma pack()

void udps_respon(int sockfd)
{
    struct sockaddr_in addr;
    int nbytes, tmplen, size_UDP_data;
    socklen_t size;
    char message[MAX_MSG];
    UDP_data *S_Msg, *R_Msg;

    while(1)
    {
        size = sizeof(struct sockaddr_in);
        nbytes = recvfrom(sockfd, &message, MAX_MSG, 0, (struct sockaddr *)&addr, &size);
        if(nbytes < 0)
        {
          printf("recvfrom(server) failed.\n");
          exit(1);
        }

        R_Msg = (UDP_data *)malloc(sizeof(UDP_data));

        memcpy(R_Msg, message, sizeof(UDP_data));
        CS_counter = R_Msg->num;
        CS_counter++;
        if((CS_counter%2) == 0)
        {
          exit(1);
        }

        printf("----------------------------------------nbytes:%d    size=%d\n", nbytes, size);    //
        printf("Server got message : {OS_name : %s ; num : %d} from IP->%s :%d\n", R_Msg->os_name, CS_counter, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        S_Msg = (UDP_data *)malloc(sizeof(UDP_data));
        S_Msg->nodeSize = sizeof(UDP_data);
        strcpy(S_Msg->os_name, os);

        S_Msg->num = CS_counter;

        nbytes = sendto(sockfd,(char *)S_Msg, S_Msg->nodeSize, 0, (struct sockaddr*)&addr, size);
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

   // printf("IP->%s :%d \n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    udps_respon(sockfd);
    close(sockfd);
}
