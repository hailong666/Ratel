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
bool CLog::isok_open = false;
std::string CLog::filename = "";
std::fstream CLog::fp;
CLog::CLog()
{
	std::string srcname = CSigleton<CConfig>::GetInstance().getParam("LogSrc"); //从配置文件中读取日志文件路径
	filename = srcname;
	fp.open(srcname.c_str(), std::fstream::out|std::fstream::app);
	if(fp.is_open()){
		isok_open = true;
	}
}
CLog::~CLog(void)
{
	fp.close();
}
void CLog::output(Log_level level, char* fmt, ...){
	if(!isok_open) assert("日志文件打开失败");

	//获取可变参数开始
	va_list va;
	va_start(va,fmt);
	int strlength = vsnprintf((char* )nullptr, 0, fmt, va);  //用来计算需要的长度
	std::string stroutput;
	stroutput.resize(strlength + 1);	//加上末尾\0
	vsnprintf((char*)stroutput.data(), stroutput.capacity(), fmt, va); //把格式化后的放到变量中
	va_end(va);
	//获取可变参数结束

	std::string strhead = makeHead(level);
	strhead += stroutput + '\n'; //日志头和日志内容相加

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
	//获取本地时间
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
	std::string str = ss.str(); //格式化时间并保存在string中

	headLine += "[" + str + "] : ";

	return headLine;
}
