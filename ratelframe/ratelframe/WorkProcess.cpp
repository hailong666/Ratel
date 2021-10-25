#include "stdafx.h"
#include "WorkProcess.h"

CWorkProcess::CWorkProcess(void)
{
}
CWorkProcess::~CWorkProcess(void)
{
}
bool CWorkProcess::initPro(void)
{
	if(!g_socket.initSocket()) return false;
	g_socket.bindSocket();
}

bool  CWorkProcess::createChild(int processnum)
{
	//int pid = fork()  Linux下才有因此在Windows中先搁下
	//if(pid < 0){
	//
	//}
	//else if(pid == 0){
	//
	//}
	//else{
	//
	//}
	return true;

}
void CWorkProcess::doWhile()
{
	g_socket.Run();
}

