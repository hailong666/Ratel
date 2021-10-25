#include "stdafx.h"
#include "ThreadMangerPool.h"

CThreadMangerPool::~CThreadMangerPool(void)
{
	{
		std::lock_guard<std::mutex> guard(threadMutex);
		for (auto& iter : threadTaskList)
		{
			iter.reset();
		}
		threadTaskList.clear();
	}
}
bool CThreadMangerPool::init(int threadnum){
	CreateThreads(threadnum);
	for(int i = 0; i < threadnum; i++){
		std::shared_ptr<CTcpClient> tmpTcpClient;
		tmpTcpClient.reset(new CTcpClient);
		tcpClients.push_back(tmpTcpClient);
	}
	is_runing = true;
	return true;
}
bool CThreadMangerPool::CreateThreads(int threadnum){

	for(int i = 0; i < threadnum; i++){
		std::shared_ptr<std::thread> tmpThread;
		tmpThread.reset(new std::thread(std::bind(&CThreadMangerPool::Run, this)));
		threadsPool.push_back(tmpThread);
	}
	return true;
}
void CThreadMangerPool::Run(){
	std::shared_ptr<ThreadTask> task;
	while(true){

		std::unique_lock<std::mutex> guard(threadMutex);

		while(threadTaskList.empty()){
			if (!is_runing)
				break;
			threadPool_cv.wait(guard);
		}
		if (!is_runing)
			break;

		task = threadTaskList.front();
		threadTaskList.pop_front();

		if (task == NULL)
			continue;

		//task->doIt();
		task.reset();
	}

}
void CThreadMangerPool::stop(){

	is_runing = false;
	for (auto& iter : threadsPool)
	{
		if (iter->joinable())
			iter->join();
	}
}
void CThreadMangerPool::addTask(ThreadTask* task){
	std::shared_ptr<ThreadTask> ptr;
	ptr.reset(task);
	{
		std::lock_guard<std::mutex> guard(threadMutex); 
		threadTaskList.push_back(ptr);
	}
	threadPool_cv.notify_all();
	
}
