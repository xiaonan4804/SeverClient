#ifndef SERVERFUNC_H_
#define SERVERFUNC_H_

#include "Photo.h"

/*线程函数参数*/
typedef struct LNode
{
	int clientfd;
	struct sockaddr_in client_addr;					//客户端口号和IP
	int flag;										//用户标记
	//struct LNode *next;
}LinkList;


int server_init(void); 		//初始化服务端
void servthread(void *arg);		//线程执行函数
void MonitorSocket(int sockfd); 	//监控套接字


#endif
