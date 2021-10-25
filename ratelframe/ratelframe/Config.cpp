/*
*  by CallMeEngineer  2021/10/24
*/

#include "stdafx.h"
#include "Config.h"
#include<fstream>
#include <string>
CConfig::CConfig(std::string filename):file_isok(false)
{
	std::ifstream fp;
	fp.open(filename.c_str(),std::ios::in);
	if(fp){
		file_isok = true;
		this->filename = filename;
	}
}
CConfig::~CConfig(void)
{
}
std::string CConfig::getParam(std::string param){
	for(auto pa : paramstore){
		if(pa.first == param)
			return pa.second;
	}
	return "";
}
int CConfig::InitConfig(){
	if(!file_isok)return -1;
	std::ifstream fp;
	fp.open(filename.c_str(), std::ifstream::in);
	if(!fp){
		std::cerr << "文件读取出错" << std::endl;
		return -1;
	}

	char str[100];
	while(fp.getline(str,100)){
		int start, end;
		for(start = 0; start < strlen(str); start++){
			if(str[start] == ' '|| str[start] == '\r' || str[start] == '\n'){
				start++;
			}else{
				break;
			}
		}
		for(end = strlen(str)-1; end > 0; end--){
			if(str[end] == ' '|| str[end] == '\r' || str[end] == '\n'){
				end--;
			}else{
				break;
			}
		}
		if(start<end){
			std::string strs(str, start, end-start + 1);
			ParamHandle(strs);
		}


	}
	if(paramstore.size() == 0){
		std::cerr << "未读取到任何参数" << std::endl;
		return -1;
	}
	return 0;
}
int CConfig::ParamHandle(std::string strline){
	int flageq = 0;
	for(int i = 0; i < strline.size(); i++){
		if(strline[0] =='#' || strline[0] == '[') return 0;
		if(strline[i] == '='){
			 flageq = i;
			 break;
		}
	}
	std::string key(strline, 0, flageq);
	std::string value(strline, flageq + 1, strline.size() - flageq);
	paramstore.insert(std::make_pair(key, value));
}