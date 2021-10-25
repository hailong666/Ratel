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
int _tmain(int argc, _TCHAR* argv[])
{
	CSigleton<CConfig>::GetInstance().InitConfig(); 

	//for(auto conf : CSigleton<CConfig>::GetInstance().InitConfig().paramstore){

	//	std::cout << conf.first << "  "  << conf.second << std::endl;

	//}
	//CLog log("test.log");
	
	CSigleton<CLog>::GetInstance().output(Log_level::LOG_DEBUG,"这是一条语句 %s %d", "哈哈哈哈哈", 1);
	CWorkProcess workpro;
	workpro.initPro();
	workpro.doWhile();
	return 0;
}

