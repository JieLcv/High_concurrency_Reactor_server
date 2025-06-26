#pragma once
#include"eventLoop.h"
#include"threadPool.h"
#include<arpa/inet.h>
#include"tcpConnection.h"
#include"Log.h"

class TcpServer
{
public:
	//��ʼ��
	TcpServer(unsigned short port, int threadNum);
	//��ʼ������
	void setListen();
	//����������
	void run();
	static int acceptConnection(void* arg);

private:
	int m_threadNum;
	EventLoop* m_mainLoop;
	ThreadPool* m_threadPool;
	int m_lfd;
	unsigned short m_port;
};

