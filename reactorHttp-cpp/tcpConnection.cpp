#include "tcpConnection.h"



TcpConnection::TcpConnection(int fd, EventLoop* evLoop)
{
	m_evLoop = evLoop;
	m_readBuf = new Buffer(10240);
	m_writeBuf = new Buffer(10240);
	//http
	m_request = new HttpRequest;
	m_response = new HttpResponse;
	m_name = "Connection-" + to_string(fd);
	m_channel = new Channel(fd, FDEvent::ReadEvent, processRead, processWrite, destroy, this);
	evLoop->addTask(m_channel, ElemType::ADD);
	Debug("和客户端建立连接， threadName:%s, connName:%s", evLoop->getThreadName().data(), m_name.data());
}

TcpConnection::~TcpConnection()
{
	if (m_readBuf && m_readBuf->readableSize() == 0 &&
		m_writeBuf && m_writeBuf->readableSize() == 0)
	{
		delete m_readBuf;
		delete m_writeBuf;
		delete m_request;
		delete m_response;
		m_evLoop->freeChannel(m_channel);
	}
	Debug("连接断开，释放资源， gameover, connName;%s",m_name.data());
}

int TcpConnection::processRead(void* arg)
{
	TcpConnection* conn = static_cast<struct TcpConnection*>(arg);

	//接受数据
	int socket = conn->m_channel->getSocket();
	int count = conn->m_readBuf->socketRead(socket);
	Debug("接受到的http请求数据：%s, count=%d", conn->m_readBuf->data(), count);
	if (count > 0)
	{
		//接受到了http请求，解析http请求
#ifdef MSG_SEND_AUTO
		conn->m_channel->writeEventEnable(true);
		//不会立刻添加读事件，因为只有一个线程，当前在执行processRead函数，执行完毕后，才会处理任务队列，然后，才开始检测
		conn->m_evLoop->addTask(conn->m_channel, ElemType::MODIFY);
#endif
		bool flag = conn->m_request->parseRequest(conn->m_readBuf, conn->m_response, conn->m_writeBuf, socket);
		//Debug("flag=%d", flag);
		if (!flag)
		{
			//解析失败，回复一个简单的html
			string errMsg = "HTTP/1.1 400 Bad Request\r\n\r\n";
			conn->m_writeBuf->appendString(errMsg);
		}
	}
	else
	{
#ifdef MSG_SEND_AUTO
		//断开连接
		conn->m_evLoop->addTask(conn->m_channel, ElemType::DELETE);
#endif
	}
#ifndef MSG_SEND_AUTO
	//断开连接
	conn->m_evLoop->addTask(conn->m_channel, ElemType::DELETE);
#endif
	return 0;
}

int TcpConnection::processWrite(void* arg)
{
	TcpConnection* conn = static_cast<struct TcpConnection*>(arg);
	//发送数据
	int count = conn->m_writeBuf->sendData(conn->m_channel->getSocket());
	if (count > 0)
	{
		Debug("开始发送数据了(基于写事件发送....), count=%d", count);
		//判断数据是否全部发送出去了
		if (conn->m_writeBuf->readableSize() == 0)
		{
			//1.不在检测写事件，--修改channel中保存的事件
			conn->m_channel->writeEventEnable(false);
			//2.修改dispatcher检测的集合--添加任务节点
			conn->m_evLoop->addTask(conn->m_channel, ElemType::MODIFY);
			//3.删除这个节点, 因此上面的两行代码可加可不加
			conn->m_evLoop->addTask(conn->m_channel, ElemType::DELETE);
			//close(conn->channel->fd);
		}
	}
	return 0;
}

int TcpConnection::destroy(void* arg)
{
	TcpConnection* conn = static_cast<struct TcpConnection*>(arg);
	if (conn != nullptr)
	{
		delete conn;
	}
	return 0;
}
