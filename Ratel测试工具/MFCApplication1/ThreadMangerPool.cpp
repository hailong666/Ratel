#pragma once
#include "stdafx.h"
#include "ThreadMangerPool.h"

CThreadMangerPool::~CThreadMangerPool(void)
{
	{
		std::lock_guard<std::mutex> guard(threadMutex);
		for (auto& iter : threadTaskList)
		{
			if(iter)
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
void ThreadTask::DoIt(){

	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		AfxMessageBox(_T("socket()失败"));
		return;
	}
	SOCKADDR_IN       server_in;
	memset(&server_in, 0, sizeof(SOCKADDR_IN));
	server_in.sin_family = AF_INET;
	server_in.sin_port = htons(80);
	server_in.sin_addr.s_addr = inet_addr("127.0.0.1");

	SOCKADDR_IN       client_in;
	memset(&client_in, 0, sizeof(SOCKADDR_IN));
	client_in.sin_family = AF_INET;
	client_in.sin_port = htons(fd);
	client_in.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(sClient, (SOCKADDR*)&client_in, sizeof(client_in)) == SOCKET_ERROR){

		std::cerr << "绑定端口出错！" << std::endl;
	}

	if (connect(sClient, (struct sockaddr *)&server_in, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		//AfxMessageBox(_T("connect()失败"));
		closesocket(sClient);
		return;
	}

	int iSendRecvTimeOut = 5000;
	if (setsockopt(sClient, SOL_SOCKET, SO_SNDTIMEO, (const char *)&iSendRecvTimeOut, sizeof(int)) == SOCKET_ERROR)
	{
		//AfxMessageBox(_T("setsockopt(SO_SNDTIMEO)失败"));
		closesocket(sClient);
		return;
	}
	if (setsockopt(sClient, SOL_SOCKET, SO_RCVTIMEO, (const char *)&iSendRecvTimeOut, sizeof(int)) == SOCKET_ERROR)
	{
		//AfxMessageBox(_T("setsockopt(SO_RCVTIMEO)失败"));
		closesocket(sClient);
		return;
	}
	while(true){};
	//AfxMessageBox(_T("非常好，connect()成功"));

}
