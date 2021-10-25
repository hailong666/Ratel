#pragma once
#include<iostream>
#include<stdarg.h>

enum class Log_level{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

class CLog
{
public:
	CLog(std::string filename);
	void output(Log_level Level, char* fmt, ...);
	~CLog(void);
private:
	std::string makeHead(Log_level Level);
private:
	bool isok_open;
	std::string filename;
};

