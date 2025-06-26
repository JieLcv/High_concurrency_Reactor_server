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
	//初始化
	Buffer(int size);
	//释放内存
	~Buffer();
	//扩容
	void bufferExtenRoom(int size);
	//得到剩余的可写内存容量
	inline int writeableSize()
	{
		return m_capacity - m_writePos;
	}
	//得到剩余的可读的内存容量
	inline int readableSize()
	{
		return m_writePos - m_readPos;
	}
	//写内存
	//1.直接写
	int appendString(const char* data, int size);
	int appendString(const char* data);
	int appendString(const string data);
	//2. 接受套接字数据
	int socketRead(int fd);
	//根据\r\n取出一行,找到其在数据块中的位置，返回该位置
	char* findCRLF();
	//发送数据
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
	//指向内存的指针
	char* m_data;
	int m_capacity;
	int m_readPos = 0;
	int m_writePos = 0;
};

