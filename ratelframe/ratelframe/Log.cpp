/*
*  by CallMeEngineer  2021/10/24
*/

#include "stdafx.h"
#include "Log.h"
#include<fstream>
#include <assert.h>
#include <stdio.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "Config.h"
#include "Singleton.h"
CLog::CLog():isok_open(false)
{
	std::string srcname = CSigleton<CConfig>::GetInstance().getParam("LogSrc");
	filename = srcname;
	std::ifstream fp(srcname.c_str(), std::ios::in);
	if(fp){
		isok_open = true;
	}
}
CLog::~CLog(void)
{
}
void CLog::output(Log_level level, char* fmt, ...){
	if(!isok_open) assert("日志文件打开失败");

	va_list va;
	va_start(va,fmt);
	int strlength = vsnprintf((char* )nullptr, 0, fmt, va);  //用来计算需要的长度
	std::string stroutput;
	stroutput.resize(strlength + 1);	//加上末尾\0
	vsnprintf((char*)stroutput.data(), stroutput.capacity(), fmt, va);
	va_end(va);

	std::string strhead = makeHead(level);
	strhead += stroutput + '\n';

	std::ofstream fp(filename.c_str(), std::ios::out | std::ios::app);

	fp.write(strhead.c_str(),strhead.size());
	
	return ;
}
std::string CLog::makeHead(Log_level level){
	std::string headLine;

	switch(level){
	case Log_level::LOG_DEBUG:
		headLine += "[Debug] ";
		break;
	case Log_level::LOG_INFO:
		headLine += "[Info] ";
		break;
	case Log_level::LOG_ERROR:
		headLine += "[Error] ";
		break;
	case Log_level::LOG_WARNING:
		headLine += "[Warning] ";
		break;
	}
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
	std::string str = ss.str();

	headLine += "[" + str + "] : ";

	return headLine;
}
