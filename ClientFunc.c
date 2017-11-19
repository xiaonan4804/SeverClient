#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/select.h>
#include "ClientFunc.h"

/*
客户端初始化，创建套接子，链接
返回套接字
*/
int client_init(char* argv)
{
	int ret;
	int sockfd;	
	struct sockaddr_in server_addr;
	

	//创建套接字
	sockfd = socket(AF_INET, SOCK_STREAM, 0);			
	if(sockfd < 0)
	{
		perror("socket()");
		exit(-1);
	}

	//建立连接
	server_addr.sin_family = AF_INET;								//使用TCP、IP
	server_addr.sin_port = htons(atoi(SERVERPORT));					//设定端口
	ret = inet_aton(argv, &server_addr.sin_addr.s_addr);		//设定IP
	if(ret == 0)
	{
		perror("inet_aton");
		exit(0);
	}
	ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));	//建立连接
	if(ret == -1)
	{
		perror("connect");
		exit(0);
	}

	return sockfd;
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
				printf(" %s\n", buf);
				memset(buf, 0, sizeof(buf));
				fflush(NULL);
			}		
			if(FD_ISSET(0, &readfds)) 								//监控标准输入
			{			
				read(0, buf, sizeof(buf));							//从键盘输入字符
				ret = send(sockfd, buf, strlen(buf), 0);				//发送数据
				if(ret == -1)
				{
					perror("send");
					continue;
				}
			}

		}

	}

}


