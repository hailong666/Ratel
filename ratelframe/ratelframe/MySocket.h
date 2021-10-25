#pragma once
#include<WinSock2.h>
#include<ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include "TcpClient.h"
#pragma comment(lib, "Ws2_32.lib")
class CMySocket
{
public:
	CMySocket(void);
	bool initSocket();
	~CMySocket(void);
	bool bindSocket(int listenport = 80);
	void Run();
	int HandleRecv(int fd, char* buf, int buflen, int i);
private:
	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKADDR_IN servAdr;
	CTcpClient tcpClient;
	std::vector<int> clientfds;
	bool ifClose;

};

