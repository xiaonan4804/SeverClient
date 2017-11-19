#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "ServerFunc.h"


/*
初始化服务端，创建套接字，绑定端口，监听端口
返回套接字，用于接收来自客户端信息
*/
int server_init(void)
{
	int ret;
	int sockfd;								//定义Sock文件描述符	
	struct sockaddr_in	host_addr;			//定义本机端口号和IP	

	/*创建套接字*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);		//使用TIP\IP，数据流套接字
	if(sockfd < 0)									//出错检查
	{
		perror("socket");
		exit(-1);
	}


	/*设置地址复用*/
	int val = 1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0)
	{
		perror("setsockopt()");
		exit(-1);
	}

	/*绑定套接字*/
	host_addr.sin_family = AF_INET;						//TCP\IP
	host_addr.sin_port = htons(atoi(SERVERPORT));			//设定端口号
	//inet_pton(AF_INET, IPADDR, &host_addr.sin_addr); 			//使用本地IP
	host_addr.sin_addr.s_addr = INADDR_ANY;				//使用本机IP
	ret = bind(sockfd, (struct sockaddr *)&host_addr, sizeof(host_addr));	//绑定端口
	if(ret < 0)
	{
		perror("bind()");
		exit(-1);
	}

	ret = listen(sockfd, 256);				//监听网络端口
	if(ret < 0)
	{
		perror("listen()");
		exit(-1);
	}

	return sockfd;
}

/*
线程主程序
*/
void servthread(void *arg)
{
	char buf[1024];
	int len;
	LinkList *client = (LinkList *)arg;

	
	strcpy(buf, "Congratulation, the secret place has been revealed!!");	

	//发送
	if(send(client->clientfd,buf,1024,0) < 0)
	{
		perror("send()");
		exit(1);
	}
	
	MonitorSocket(client->clientfd);
}

/*
监控套接字，服务端有数据发送，接收到执行操作。
监控标准输入，有键盘输入时，发送消息。
*/
void MonitorSocket(int sockfd)
{
	fd_set readfds; 	//读文件描述符集
	int ret;
	char buf[1024];
	while(1)
	{
		FD_ZERO(&readfds);			//清空描述符集
		FD_SET(0, &readfds);		//将键盘fd加入读文件描述符集
		FD_SET(sockfd, &readfds);	//将sockfd加入读文件描述符集
		ret = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);			//监视文件
		if(ret <= 0)
		{
			perror("select");
			continue;
		}
		else if (ret > 0)
		{
			if(FD_ISSET(sockfd, &readfds))								//套接字文件状态发生变化
			{						
				memset(buf, 0, sizeof(buf));
				ret = recv(sockfd, buf, sizeof(buf), 0);					//接收数据
				if(ret == -1)
				{
					perror("recv");
					continue;
				}
				printf("message from Client: %s\n", buf);
				memset(buf, 0, sizeof(buf));
				strcpy(buf, "Greatings, How may I serve you.");	
				if(send(sockfd,buf,1024,0) < 0)
				{
					perror("send()");
					exit(1);
				}
				
			}		


		}

	}

}


