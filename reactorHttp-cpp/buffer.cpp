#include "buffer.h"



Buffer::Buffer(int size):m_capacity(size)
{
	//初始化方式的优先级 就地初始化 > 初始化列表 > 构造函数中初始化，优先级低的会覆盖高的
	m_data = (char*)malloc(size);
	bzero(m_data, size);
}

Buffer::~Buffer()
{
	if (m_data != nullptr)
	{
		free(m_data);
	}
}

void Buffer::bufferExtenRoom(int size)
{
	//1. 内存够用，不需要扩容
	if (writeableSize() >= size)
	{
		return;
	}
	//2. 内存需要合并才够用，不需要扩容, 剩余可写的内存 + 已读的内存 > size
	if (m_readPos + writeableSize() >= size)
	{
		//得到未读内存的大小
		int readable = readableSize();
		//移动内存
		memcpy(m_data, m_data + m_readPos, readable);
		//更新位置
		m_readPos = 0;
		m_writePos = readable;
	}
	//内存不够用,扩容
	else
	{
		void* tmp = realloc(m_data, m_capacity + size);
		if (tmp == NULL)return;
		memset((char*)tmp + m_capacity, 0, size);
		m_data = static_cast<char*>(tmp);
		m_capacity += size;
	}
}

int Buffer::appendString(const char* data, int size)
{
	if (m_data == nullptr || size <= 0)
	{
		return -1;
	}
	//扩容
	bufferExtenRoom(size);
	//数据拷贝  strcpy, strlen遇到'/0'结束
	memcpy(m_data + m_writePos, data, size);
	//Debug("正在拷贝数据:%s", buffer->data + buffer->writePos);
	m_writePos += size;
	return 0;
}

int Buffer::appendString(const char* data)
{
	int size = strlen(data);
	int ret = appendString(data, size);
	return ret;
}

int Buffer::appendString(const string data)
{
	int ret = appendString(data.data());
	return ret;
}

int Buffer::socketRead(int fd)
{
	//read/recv/readv
	struct iovec vec[2];
	int writeable = writeableSize();
	vec[0].iov_base = m_data + m_writePos;
	vec[0].iov_len = writeable;
	char* tmpbuf = (char*)malloc(40960);
	vec[1].iov_base = tmpbuf;
	vec[1].iov_len = 40960;
	int result = readv(fd, vec, 2);

	if (result == -1)
	{
		return -1;
	}
	else if (result <= writeable)
	{
		m_writePos += result;
	}
	else
	{
		m_writePos = m_capacity;
		appendString(tmpbuf, result - writeable);
	}
	free(tmpbuf);
	//Debug("正在读数据:%s, result=%d, writable=%d, writePos=%d, readPos=%d", tmpbuf, result, writeable, buffer->writePos, buffer->readPos);
	return result;
}

char* Buffer::findCRLF()
{
	//strstr -->大字符串中匹配子字符串（遇到\0结束）
	//memmem -->大数据库中匹配子数据块(需要指定数据块大小，遇到\0不结束）
	char* ptr = (char*)memmem(m_data + m_readPos, readableSize(), "\r\n", 2);
	return ptr;
}

int Buffer::sendData(int socket)
{
	//判断有无数据
	int readable = readableSize();
	if (readable > 0)
	{
		int count = send(socket, m_data + m_readPos, (size_t)readable, 0);
		if (count > 0)
		{
			m_readPos += (int)count;
			usleep(1);
			return count;
		}
	}
	return 0;
}
