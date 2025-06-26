#pragma once
//#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<sys/uio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string>
#include"Log.h"
using namespace std;
class Buffer
{
public:
	//��ʼ��
	Buffer(int size);
	//�ͷ��ڴ�
	~Buffer();
	//����
	void bufferExtenRoom(int size);
	//�õ�ʣ��Ŀ�д�ڴ�����
	inline int writeableSize()
	{
		return m_capacity - m_writePos;
	}
	//�õ�ʣ��Ŀɶ����ڴ�����
	inline int readableSize()
	{
		return m_writePos - m_readPos;
	}
	//д�ڴ�
	//1.ֱ��д
	int appendString(const char* data, int size);
	int appendString(const char* data);
	int appendString(const string data);
	//2. �����׽�������
	int socketRead(int fd);
	//����\r\nȡ��һ��,�ҵ��������ݿ��е�λ�ã����ظ�λ��
	char* findCRLF();
	//��������
	int sendData(int socket);
	inline char* data()
	{
		return m_data + m_readPos;
	}
	inline int readPosIncrease(int count)
	{
		m_readPos += count;
		return m_readPos;
	}

private:
	//ָ���ڴ��ָ��
	char* m_data;
	int m_capacity;
	int m_readPos = 0;
	int m_writePos = 0;
};

