#pragma once
#include"eventLoop.h"
#include"threadPool.h"
#include<arpa/inet.h>
#include"tcpConnection.h"
#include"Log.h"

class TcpServer
{
public:
	//初始化
	TcpServer(unsigned short port, int threadNum);
	//初始化监听
	void setListen();
	//启动服务器
	void run();
	static int acceptConnection(void* arg);

private:
	int m_threadNum;
	EventLoop* m_mainLoop;
	ThreadPool* m_threadPool;
	int m_lfd;
	unsigned short m_port;
};

