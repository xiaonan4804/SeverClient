#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include "Photo.h"
#include "ServerFunc.h"


int main(void)
{
	int sockfd;
	int clientfd;
	int length;
	char ipstr[32];	//保存ip地址字符串

	pthread_t tid;	//线程ID
	LinkList* NewClient;	//线程输入参数

	struct sockaddr_in client_addr; 	//客户端IP端口号
	length = sizeof(client_addr);

	sockfd = server_init(); 	//初始化Socket（创建套接子，绑定端口，监听端口）
		
	while(1)
	{
		/*接收链接请求*/
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &length);
		if(clientfd == -1)
		{			
			perror("accept");
			continue;
		}

		/*打印客户端IP及端口号*/
		inet_ntop(AF_INET,&client_addr.sin_addr,ipstr,32);
		printf("Client connet: %s : %d\n", ipstr, ntohs(client_addr.sin_port));

		NewClient = (LinkList *)malloc(sizeof(LinkList));
		NewClient->clientfd = clientfd;
		NewClient->client_addr = client_addr;
		pthread_create(&tid, NULL, (void *)servthread, NewClient); 		//创建执行线程
		//close(clientfd);
	}

	close(sockfd);

	exit(0);
}


