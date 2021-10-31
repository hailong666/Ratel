#pragma once

#ifdef WIN32
#include<WinSock2.h>
#include<ws2tcpip.h>
#else
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#endif
/*
*  by CallMeEngineer  2021/10/24
*/

#include <iostream>
#include <string>
#include <vector>
#include "TcpClient.h"
#include "ThreadMangerPool.h"
#include "Log.h"
#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif
class CMySocket
{
public:
	CMySocket(void);
	bool initSocket();
	~CMySocket(void);
	bool bindSocket(int listenport = 80);
	void Run();
	int HandleRecv(int fd, char* buf, unsigned int buflen, int i);

private:
#ifdef WIN32
	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKADDR_IN servAdr;
#else
	int serverSocket;	
	struct sockaddr_in servAdr;
#endif
	static CTcpClient tcpClient;	
	std::vector<int> clientfds;
	bool ifClose;
	CThreadMangerPool threadPool;

};

