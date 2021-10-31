/*
*  by CallMeEngineer  2021/10/24
*/

#pragma once

#ifdef WIN32
#include<WinSock2.h>
#include<ws2tcpip.h>
#else
#include<arpa/inet.h>
#endif

#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <functional>
#include "TcpClient.h"
#include "httpHandle.h"

#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

class ThreadTask{
public:
	ThreadTask(char* data, unsigned int len);
	char* bodyData;
	unsigned int len;
	void DoIt(void);
};
class CThreadMangerPool
{
public:
	CThreadMangerPool(void):is_runing(false){};
	bool init(int threadnum);
	~CThreadMangerPool(void);
	void Run(void);
	void stop(void);
	void addTask(ThreadTask* task);
private:
	bool CreateThreads(int threadnum = 5);
	std::vector<std::shared_ptr<std::thread>> threadsPool;
	std::list<std::shared_ptr<ThreadTask>>    threadTaskList;
	std::condition_variable					  threadPool_cv;
	std::mutex								  threadMutex;
	//std::vector<std::shared_ptr<CTcpClient>>  tcpClients;
	bool is_runing;
};

