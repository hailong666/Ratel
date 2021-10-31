/*
*  by CallMeEngineer  2021/10/30
*/

#include "stdafx.h"
#include <sstream>
#include <vector>
#include "httpHandle.h"


ChttpHandle::ChttpHandle(char* data,unsigned int len):httpData(data),dataLen(len)
{

}
ChttpHandle::~ChttpHandle(void)
{
}
bool ChttpHandle::handleData(void){
	for( int i = 0; i < dataLen; i++){
		std::string str("");
		str.clear();
		for(int j = i; j < dataLen; j++){
			if(httpData[j] != '\r'){
				str += httpData[j];
			}else{
				if(j + 1 < dataLen && httpData[j + 1] == '\n'){

					if( j + 2 < dataLen && httpData[j + 2] == '\r'){

						if(j + 3 < dataLen && httpData[j + 3] == '\n'){
							std::pair<std::string, std::string> param = paramLine(str.substr(0,str.size()));
							if(param != std::pair<std::string, std::string>("","")){
								paramMap.insert(param);
							}
							if(headline.RequestMode == std::string("POST")){
								return getPostValue(j + 4);
							}
							return true;
						}
						else{
							std::cerr << "http包数据出错！" << std::endl;
							return false;
						}
					}
					else{
						std::pair<std::string, std::string> param = paramLine(str);
						if(param != std::pair<std::string, std::string>("","")){
							paramMap.insert(param);
						}
						i = j + 1;
						break;
					}

				}else{
					std::cerr << "http包数据有误！" << std::endl;
					return false;
				}
			}

		}		
	}
	return false;
}
std::pair<std::string, std::string> ChttpHandle::paramLine(std::string line){

	std::stringstream linedata(line);
	std::vector<std::string> vecstr;
	std::string strtmp;
	while(linedata >> strtmp){
		vecstr.push_back(strtmp);
	}
	if(vecstr.size() == 3){
		if(vecstr[0] == std::string("GET") || vecstr[0] == std::string("POST")){
			headline.RequestMode = vecstr[0];
			headline.RequestPath = vecstr[1];
			headline.RequestVersion = vecstr[2];

		}else{
			return std::pair<std::string, std::string>("","");
		}
	}
	else if(vecstr.size() == 1){

		std:: string strparam = vecstr[0].substr(0, vecstr[0].size());
		auto ptr =  strparam.find_first_of(':');
		std::string secondstr(strparam, ptr + 1); 
		std::string firststr = strparam.substr(0,ptr);
		return std::pair<std::string, std::string>(firststr,secondstr);
	}
	else{
		return std::pair<std::string, std::string>("","");
	}
	return std::pair<std::string, std::string>("","");
}
bool ChttpHandle::HandleIt(){

	if(handleData()){
	
		if(headline.RequestMode == std::string("GET")){

			return handleGet();

		}else if(headline.RequestMode == std::string("POST")){

			return handleData();

		}else{
			//TODO:处理其他请求类型
		}

	}else{
		return false;
	}

	return true;
}
bool ChttpHandle::getPostValue(unsigned int posdata){
	std::vector<int> vecpapam;
	vecpapam.push_back(-1);
	std::string strs(httpData, dataLen);
	strs = strs.substr(posdata, dataLen - posdata + 1);
	for(int i = posdata; i < dataLen; i++ ){
		if(httpData[i] == '&' || httpData[i] == '='){
			vecpapam.push_back(i - posdata);
		}
	}
	vecpapam.push_back(dataLen - posdata);

	if((vecpapam.size() & 1 ) && (((vecpapam.size() -1) >> 1) & 1) == 0 ){
		
		bool flag = true;
		std::vector<std::string> vecfirst;
		std::vector<std::string> vecsecond;
		for(int i = 1 ; i < vecpapam.size(); i++){
			if(flag){
				std::string tmp = strs.substr(vecpapam[i-1] + 1, vecpapam[i] - vecpapam[i-1] - 1 );
				vecfirst.push_back(tmp);
				flag = !flag;
			}else{
				std::string tmp = strs.substr(vecpapam[i-1] + 1, vecpapam[i] - vecpapam[i-1] - 1 );
				vecsecond.push_back(tmp);
				flag = !flag;
			}
		}
		for(int i = 0; i < vecfirst.size(); i++){
			paramPostMap.insert(std::make_pair(vecfirst[i],vecsecond[i]));
		}
		return true;

	}else{
		return false;
	}
}
bool ChttpHandle::handleGet(){
	std::cout << "处理GET请求...." << std::endl;
	return true;
}
bool ChttpHandle::handlePost(){
	std::cout << "处理POST请求...." << std::endl;
	return true;
}