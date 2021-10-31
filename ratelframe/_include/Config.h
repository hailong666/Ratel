/*
*  by CallMeEngineer  2021/10/24
*/

#pragma once
#include<iostream>
#include<map>
class CConfig
{
public:
	//�����ļ���·��
	CConfig(std::string filename = "test.conf");
	~CConfig(void);
	//��ʼ������
	int InitConfig();
	//��ȡ���ò���
	std::string getParam(std::string param);
	//�������ò���
	std::string setParam();
private:
	
	bool file_isok; //�ļ��Ƿ��ȡ�ɹ�
	int ParamHandle(std::string strline); //����ÿһ�еĲ���
public:
	std::map<std::string,std::string> paramstore; // �������в���
	std::string filename; //�ļ���
	
};

