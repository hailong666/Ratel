/*
*  by CallMeEngineer  2021/10/24
*/

#pragma once
#include<iostream>
#include<stdarg.h>

enum class Log_level{
	LOG_DEBUG,		 //调试输出
	LOG_INFO,		//信息输出
	LOG_WARNING,	//警告信息
	LOG_ERROR		//错误信息
};
//宏定义用来简化调用，使用起来更方便
#define LOG(Level,...) CLog::output(Level, __VA_ARGS__)

#define LOGD(...) LOG(Log_level::LOG_DEBUG, __VA_ARGS__)
#define LOGI(...) LOG(Log_level::LOG_INFO, __VA_ARGS__)
#define LOGW(...) LOG(Log_level::LOG_WARNING, __VA_ARGS__)
#define LOGE(...) LOG(Log_level::LOG_ERROR, __VA_ARGS__)
class CLog
{
public:
	CLog();
	static void output(Log_level Level, char* fmt, ...); //主要的输出函数
	~CLog(void);
private:
	static std::string makeHead(Log_level Level); //用来构建日志的头部输出

	static bool isok_open;	//标记日志是否成功打开
	static std::string filename;	//日志文件路径，从配置文件中获取
	static std::fstream fp;
};

