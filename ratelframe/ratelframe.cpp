// ratelframe.cpp : 定义控制台应用程序的入口点。
//

/*
*  by CallMeEngineer  2021/10/24
*/

#include "stdafx.h"
#include <string>
#include <iostream>
#include "Config.h"
#include "Log.h"
#include "WorkProcess.h"
#include "Singleton.h"
#ifdef WIN32
#include <tchar.h>
#endif

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char** argv)
#endif
{
	CSigleton<CConfig>::GetInstance().InitConfig(); 
	
	CSigleton<CLog>::GetInstance().output(Log_level::LOG_DEBUG, (char*)"hello world! %s %d","hahahaha",1);
	CWorkProcess workpro;
	workpro.initPro();
	workpro.doWhile();
	return 0;
}

