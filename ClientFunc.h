#ifndef CLIENTFUNC_H_
#define CLIENTFUNC_H_

#include "Photo.h"

int client_init(char* argv); 	//客户端初始化
void MonitorSocket(int sockfd); 	//监控套接字

#endif
