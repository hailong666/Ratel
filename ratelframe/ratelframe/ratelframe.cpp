// ratelframe.cpp : �������̨Ӧ�ó������ڵ㡣
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
int _tmain(int argc, _TCHAR* argv[])
{
	CSigleton<CConfig>::GetInstance().InitConfig(); 

	//for(auto conf : CSigleton<CConfig>::GetInstance().InitConfig().paramstore){

	//	std::cout << conf.first << "  "  << conf.second << std::endl;

	//}
	//CLog log("test.log");
	
	CSigleton<CLog>::GetInstance().output(Log_level::LOG_DEBUG,"����һ����� %s %d", "����������", 1);
	CWorkProcess workpro;
	workpro.initPro();
	workpro.doWhile();
	return 0;
}

