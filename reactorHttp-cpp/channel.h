#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<functional>
//���庯��ָ��
//����ָ�����ָ����ͨ���������о�̬�����������ֺ���ֻҪ����������������ʹ����ˣ���������ͨ��Ա������ֻ��ʵ������Ŵ���
//������ͨ��Ա�������ڶ���ģ�����̬���������ڶ���������
//typedef int(*handleFunc)(void* arg);
//using handleFunc = int(*)(void* arg);

//�����ļ��������Ķ�д�¼�, ǿ����ö�٣�ʹ��ʱ��Ҫ����ö������::, ��˿����ڲ�ͬ��ǿ����ö���ڶ�����ͬ������, ����ǿ����ö��ʱ��class��structû����
//ʹ�������ֵʱ��ֵ��������ö�ٵ����ͣ�������int���ܽ�����ʽ����ת��
enum class FDEvent
{
	TimeOut = 0x01,
	ReadEvent = 0x02,
	WriteEvent = 0x04
};


//�ɵ��ö����װ���������ʲô��1.����ָ��(���ֺ����� 2.�ɵ��ö���(��������һ��ʹ�ã�����º���)
// ���յõ��˵�ַ������û�е���
// Ҳ���Խ��޲κ�����װΪ�вκ�����ͨ����װ������ʱ����ָ������
// �ɵ��ö����װ���������ܰ󶨷Ǿ�̬��Ա������ͨ�������󶨺󣬾Ϳ��Ա��ɵ��ö����װ����װ��
//�ļ�������ͨ��
class Channel
{
public:
	using handleFunc = std::function<int(void*)>;
	//��ʼ��һ��Channel
	Channel(int fd, FDEvent events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);
	//�޸�fd��д�¼������ or ����⣩
	void writeEventEnable(bool flag);
	//�ж��Ƿ���Ҫ����ļ���������д�¼�
	bool isWriteEventEnable();
	//ȡ��˽�г�Ա��ֵ
	//�����������ô�������ʱ����Ҫ������ջ��ջ������ֱ�ӽ��д�����滻��ִ��Ч�ʸߣ�������Ҫ���ĸ����ڴ�
	inline int getEvent()
	{
		return m_events;
	}
	inline int getSocket()
	{
		return m_fd;
	}
	//const ���Ͳ��ܴ����const �βΣ���Ҫ��������ת��
	inline const void* getArg()
	{
		return m_arg;
	}
	//�ص�����
	handleFunc readCallback;
	handleFunc writeCallback;
	handleFunc destroyCallback;

private:
	//�ļ�������
	int m_fd;
	//�¼�
	int m_events;
	//�ص������Ĳ���
	void* m_arg;
};

