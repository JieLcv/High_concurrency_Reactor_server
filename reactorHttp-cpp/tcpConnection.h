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
	//初始化
	TcpConnection(int fd, struct EventLoop* evLoop);
	//资源释放
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
	//http协议
	HttpRequest* m_request;
	HttpResponse* m_response;
};
