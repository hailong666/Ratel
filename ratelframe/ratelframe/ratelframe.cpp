// ratelframe.cpp : �������̨Ӧ�ó������ڵ㡣
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
	log.output(Log_level::LOG_DEBUG,"����һ����� %s %d", "����������", 1);
	CWorkProcess workpro;
	workpro.initPro();
	workpro.doWhile();
	return 0;
}

