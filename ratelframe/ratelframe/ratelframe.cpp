// ratelframe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "Config.h"
#include "Log.h"
#include "WorkProcess.h"
int _tmain(int argc, _TCHAR* argv[])
{
	CConfig config("E://Linux//frame_cpp//ratelframe//Debug//test.conf");
	config.InitConfig();
	for(auto conf : config.paramstore){

		std::cout << conf.first << "  "  << conf.second << std::endl;

	}
	CLog log("test.log");
	log.output(Log_level::LOG_DEBUG,"这是一条语句 %s %d", "哈哈哈哈哈", 1);
	CWorkProcess workpro;
	workpro.initPro();
	workpro.doWhile();
	return 0;
}

