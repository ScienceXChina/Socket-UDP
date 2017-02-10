#include <rtthread.h>
#include <string.h>
#include <lwip/netdb.h> /* 为了解析主机名，需要包含netdb.h头文件 */
#include <lwip/sockets.h> /* 使用BSD socket，需要包含sockets.h头文件 */
#include <stdlib.h>

#define MAX_MSG 1024//#define MAX_MSG 1024

//const char send_data[] = "This is UDP Client from RT-Thread.\n"; /* 发送用到的数据 */
const char *os = "RT_thread"; 
unsigned int CS_counter = 0;

#pragma	pack(1)
typedef struct{
	int nodeSize; //4
	char os_name[20]; //20
	unsigned int num;//4
}UDP_data;
#pragma pack()

void udpclient(const char* url, int port, int count)
{
   int sockfd, size_UDP_data;
   struct hostent *host;
   struct sockaddr_in server_addr;
	 char recvbuff[MAX_MSG];
	 int send_nbytes, recv_nbytes;
	 socklen_t size;
	 UDP_data *S_Msg,*R_Msg;
	
   /* 通过函数入口参数url获得host地址（如果是域名，会做域名解析）*/
   host= (struct hostent *) gethostbyname(url);

   /* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
   if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
   {
       rt_kprintf("Socket error\n");
       return;
   }

   /* 初始化预连接的服务端地址 */
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(port);
   server_addr.sin_addr = *((struct in_addr *)host->h_addr);
   rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
	 
	 /* connect to server */
	 if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	 {
		 rt_kprintf("connect error");
		 exit(1);
	 }
	 rt_kprintf("Connet server IP -> %s :%d \n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
		 
	 size = sizeof(server_addr);
   /* 总计发送count次数据 */
	 //rt_thread_delay(300);
	 S_Msg = (UDP_data *)rt_malloc(sizeof(UDP_data));
	 
   while (count)
   {
       /* 发送数据到服务远端 */
//       send_nbytes = sendto(sockfd, send_data, strlen(send_data), 0,
//              (struct sockaddr *)&server_addr, sizeof(struct sockaddr));	
		 
	     //S_Msg = (UDP_data *)malloc(sizeof(UDP_data));
		   //S_Msg = (UDP_data *)rt_malloc(sizeof(UDP_data));
		 
			 S_Msg->nodeSize = sizeof(UDP_data);
		   rt_kprintf("S_Msg->nodeSize :%d\n", S_Msg->nodeSize);
		 
			 strcpy(S_Msg->os_name, os);
		   S_Msg->num = CS_counter;
		   //rt_kprintf("S_Msg->num = %d\n", S_Msg->num);

		   send_nbytes = write(sockfd, (char *)S_Msg, S_Msg->nodeSize);
       if (send_nbytes < 0)
			 {
				 rt_kprintf("(Client)sending error.\n");
				 exit(1);
			 }
		  			 
			 /* 接收数据到本地客户端 */
//			 recv_nbytes = recvfrom(sockfd, recvbuff, MAX_MSG, 0, (struct sockaddr*)&server_addr, &size);
			 recv_nbytes = read(sockfd, recvbuff, MAX_MSG);
			 if (recv_nbytes < 0)
			 {
				 rt_kprintf("(Client)receiving error.\n");
				 exit(1);
			 }
			 
			 recvbuff[recv_nbytes] = 0;
			 R_Msg = (UDP_data *)recvbuff;
			 CS_counter = R_Msg->num;
			 CS_counter++;
			 
			 if((CS_counter%2) != 0)
			 {
				 exit(1);
			 }
			 
			 rt_kprintf("Client received message: {OS_name : %s ; num : %d} from IP -> %s :%d \n", R_Msg->os_name, CS_counter, inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
			 
			 //rt_kprintf(" IP -> %s :%d \n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
			 
       /* 线程休眠一段时间 */
       rt_thread_delay(0);

			 //rt_free((void *)S_Msg);
			 //free((void *)S_Msg);
			 
       /* 计数值减一 */
       count --;
   }

   /* 关闭这个socket */
   closesocket(sockfd);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
/* 输出udpclient函数到finsh shell中 */
FINSH_FUNCTION_EXPORT(udpclient, startup udp client);
#endif
