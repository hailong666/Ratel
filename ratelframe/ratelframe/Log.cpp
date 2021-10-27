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
	std::string srcname = CSigleton<CConfig>::GetInstance().getParam("LogSrc"); //�������ļ��ж�ȡ��־�ļ�·��
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
	if(!isok_open) assert("��־�ļ���ʧ��");

	//��ȡ�ɱ������ʼ
	va_list va;
	va_start(va,fmt);
	int strlength = vsnprintf((char* )nullptr, 0, fmt, va);  //����������Ҫ�ĳ���
	std::string stroutput;
	stroutput.resize(strlength + 1);	//����ĩβ\0
	vsnprintf((char*)stroutput.data(), stroutput.capacity(), fmt, va); //�Ѹ�ʽ����ķŵ�������
	va_end(va);
	//��ȡ�ɱ��������

	std::string strhead = makeHead(level);
	strhead += stroutput + '\n'; //��־ͷ����־�������

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
	//��ȡ����ʱ��
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
	std::string str = ss.str(); //��ʽ��ʱ�䲢������string��

	headLine += "[" + str + "] : ";

	return headLine;
}
