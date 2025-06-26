#pragma once
#include"dispatcher.h"
#include<thread>
#include<queue>
#include<map>
#include<mutex>
#include<assert.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include"Log.h"
using namespace std;

//����ýڵ��е�channel�ķ�ʽ
enum class ElemType:char { ADD, DELETE, MODIFY };
//����������еĽڵ�
struct ChannelElement
{
	ElemType type;    //��δ���ýڵ��е�channel
	Channel* channel;
};
//��ֹDispatcher��EventLoop�໥����
class Dispatcher;
class EventLoop
{
public:
	//�ͷ�
	~EventLoop();
	//��ʼ��
	EventLoop();
	EventLoop(const string threadName);
	//��ʼ�����̣߳�ָ�����߳�����
	//������Ӧ��ģ��
	int run();
	//��������ļ�fd
	int eventActivate(int fd, int event);
	//��������������
	int addTask(struct Channel* channel, ElemType type);
	//������������е�����
	int processTaskQ();
	//����dispatcher�еĽڵ�
	int add(Channel* channel);
	int remove(Channel* channel);
	int modify(Channel* channel);
	//�ͷ�channel
	int freeChannel(struct Channel* channel);
	static int readLocalMessage(void *arg);
	int readMessage();
	//�����߳�ID
	inline thread::id getThreadID()
	{
		return m_threadID;
	}
	inline string getThreadName()
	{
		return m_threadName;
	}

private:
	bool m_isQuit;
	//��ָ��ָ�������ʵ�� epoll, poll, select
	Dispatcher* m_dispatcher;
	//�������
	queue<ChannelElement*> m_taskQ;
	//map����ļ���������channel��ӳ���ϵ
	map<int, Channel*> m_channelMap;
	//�߳�id�� name, nutex
	thread::id m_threadID;
	string m_threadName;
	mutex m_mutex;
	int m_socketPair[2];  //�洢����ͨ�ŵ�fd ͨ��socketpair��ʼ��
	void taskWakeup();
};
