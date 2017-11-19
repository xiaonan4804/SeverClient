#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/select.h>
#include "Photo.h"
#include "ClientFunc.h"


int main(int argc,char **argv)
{
	int sockfd;
	//long long stamp;  	//时间戳
	char buf[1024];
	int len;

	if(argc != 2)		//命令行检查，参数IP地址
	{
		printf("input error: [%s] [IP]\n",argv[0]);
		exit(0);
	}

	sockfd = client_init(argv[1]);	//初始化套接字

	MonitorSocket(sockfd); 		//监控套接字

/*
	while(1)
	{
		len = read(sockfd,buf,1024);
		if(len < 0)
		{
			perror("read()");
			exit(1);	
		}

		printf("message from server: %s\n", buf);

	}
*/
	close(sockfd);


	exit(0);
}
