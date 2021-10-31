/*
*  by CallMeEngineer  2021/10/24
*/

#pragma once
#include<iostream>
#include<map>
class CConfig
{
public:
	//配置文件的路径
	CConfig(std::string filename = "test.conf");
	~CConfig(void);
	//初始化配置
	int InitConfig();
	//获取配置参数
	std::string getParam(std::string param);
	//设置配置参数
	std::string setParam();
private:
	
	bool file_isok; //文件是否读取成功
	int ParamHandle(std::string strline); //处理每一行的参数
public:
	std::map<std::string,std::string> paramstore; // 保存所有参数
	std::string filename; //文件名
	
};

