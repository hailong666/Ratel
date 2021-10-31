/*
*  by CallMeEngineer  2021/10/24
*/

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

	return true;

}
void CWorkProcess::doWhile()
{
	g_socket.Run();
}

