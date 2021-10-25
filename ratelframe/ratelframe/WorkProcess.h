#pragma once
#include "MySocket.h"
class CWorkProcess
{
public:
	CWorkProcess(void);
	bool initPro(void);
	void doWhile(void);
	~CWorkProcess(void);
private:
	bool createChild(int processnum = 4);
	CMySocket g_socket;
};

