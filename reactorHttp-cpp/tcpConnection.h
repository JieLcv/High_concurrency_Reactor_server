#pragma once
#include"eventLoop.h"
#include"channel.h"
#include"buffer.h"
#include"httpRequest.h"
#include"httpResponse.h"
#include"Log.h"
using namespace std;
class TcpConnection
{
public:
	//��ʼ��
	TcpConnection(int fd, struct EventLoop* evLoop);
	//��Դ�ͷ�
	~TcpConnection();
	static int processRead(void* arg);
	static int processWrite(void* arg);
	static int destroy(void* arg);
private:
	EventLoop* m_evLoop;
	Channel* m_channel;
	Buffer* m_readBuf;
	Buffer* m_writeBuf;
	string m_name;
	//httpЭ��
	HttpRequest* m_request;
	HttpResponse* m_response;
};
