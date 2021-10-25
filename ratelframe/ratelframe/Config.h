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
	//��ʼ������
	int InitConfig();
	//��ȡ���ò���
	std::string getParam(std::string param);
	//�������ò���
	std::string setParam();
private:
	//����ÿһ�еĲ���
	bool file_isok;
	int ParamHandle(std::string strline);
public:
	std::map<std::string,std::string> paramstore;
	std::string filename;
	
};

