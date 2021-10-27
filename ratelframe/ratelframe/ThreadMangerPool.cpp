/*
*  by CallMeEngineer  2021/10/24
*/

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
	is_runing = true;
	CreateThreads(threadnum);
	//for(int i = 0; i < threadnum; i++){
	//	std::shared_ptr<CTcpClient> tmpTcpClient;
	//	tmpTcpClient.reset(new CTcpClient);
	//	tcpClients.push_back(tmpTcpClient);
	//}
	
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

		task->DoIt();
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
ThreadTask::ThreadTask(char* data, unsigned int len){
	bodyData = data + DATA_BUFSIZE;
	this->len = len;
}
void ThreadTask::DoIt(){
	std::cout << "处理了一条数据长度为：" << this->len << std::endl;
	char* yonghuming = bodyData + 4;
	char* mima = bodyData + 60;
	unsigned long* ModeType =  (unsigned long*)bodyData;
	unsigned long modetype = ntohl(*ModeType);
	
	std::cout << "该数据的模式为：" << modetype << std::endl;
	std::cout << "用户名为 ：" << yonghuming << std::endl;
	std::cout << "密码为   ：" << mima << std::endl;

}
