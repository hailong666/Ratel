/*
*  by CallMeEngineer  2021/10/30
*/
#pragma once
#include <iostream>
#include <map>
#include <string>
class ChttpHandle
{
public:
	ChttpHandle(char* data,unsigned int len);
	~ChttpHandle(void);
	bool HandleIt();
private:
	bool handleData(void);
	bool handleGet();
	bool handlePost();
	bool getPostValue(unsigned int posdata);
	std::pair<std::string, std::string> paramLine(std::string line);
	std::map<std::string , std::string> paramMap;
	std::map<std::string, std::string> paramPostMap;
	char* httpData;
	unsigned int dataLen;
	struct HEADLEN{
		std::string RequestMode;
		std::string RequestPath;
		std::string RequestVersion;
	}headline;
};

