#pragma once
#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>
#include "TcpClient.h"
class ThreadTask{

};
class CThreadMangerPool
{
public:
	CThreadMangerPool(void):is_runing(false){}
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
	std::vector<std::shared_ptr<CTcpClient>>  tcpClients;
	bool is_runing;
};

