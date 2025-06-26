#pragma once
#include"eventLoop.h"
#include"workThread.h"
#include<assert.h>
#include<vector>
using namespace std;
//�����̳߳�
class ThreadPool
{
public:
	//��ʼ���̳߳�
	ThreadPool(EventLoop* mainLoop, int count);
	//�ͷ��̳߳�
	~ThreadPool();
	//�����̳߳�
	void run();
	//ȡ���̳߳��е�ĳ�����̵߳ķ�Ӧ��ʵ��
	EventLoop* takeWorkerEventLoop();
private:
	//���̵߳ķ�Ӧ��ģ��
	EventLoop* m_mainLoop;
	bool m_isStart;
	int m_threadNum;
	vector<WorkerThread*> m_workerThreads;
	int m_index;
};


