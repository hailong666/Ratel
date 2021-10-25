/*
*  by CallMeEngineer  2021/10/24
*/

#pragma once
#include<iostream>
#include<map>
class CConfig
{
public:
	CConfig(std::string filename = "test.conf");
	~CConfig(void);
	//初始化配置
	int InitConfig();
	//获取配置参数
	std::string getParam(std::string param);
	//设置配置参数
	std::string setParam();
private:
	//处理每一行的参数
	bool file_isok;
	int ParamHandle(std::string strline);
public:
	std::map<std::string,std::string> paramstore;
	std::string filename;
	
};

