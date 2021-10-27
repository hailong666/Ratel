/*
*  by CallMeEngineer  2021/10/24
*/

#pragma once
#include<iostream>
#include<stdarg.h>

enum class Log_level{
	LOG_DEBUG,		 //�������
	LOG_INFO,		//��Ϣ���
	LOG_WARNING,	//������Ϣ
	LOG_ERROR		//������Ϣ
};
//�궨�������򻯵��ã�ʹ������������
#define LOG(Level,...) CLog::output(Level, __VA_ARGS__)

#define LOGD(...) LOG(Log_level::LOG_DEBUG, __VA_ARGS__)
#define LOGI(...) LOG(Log_level::LOG_INFO, __VA_ARGS__)
#define LOGW(...) LOG(Log_level::LOG_WARNING, __VA_ARGS__)
#define LOGE(...) LOG(Log_level::LOG_ERROR, __VA_ARGS__)
class CLog
{
public:
	CLog();
	static void output(Log_level Level, char* fmt, ...); //��Ҫ���������
	~CLog(void);
private:
	static std::string makeHead(Log_level Level); //����������־��ͷ�����

	static bool isok_open;	//�����־�Ƿ�ɹ���
	static std::string filename;	//��־�ļ�·�����������ļ��л�ȡ
	static std::fstream fp;
};

