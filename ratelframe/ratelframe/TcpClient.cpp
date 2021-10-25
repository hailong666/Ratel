/*
*  by CallMeEngineer  2021/10/24
*/

#include "stdafx.h"
#include "TcpClient.h"
#include <windows.h>
#pragma comment(lib, "wsock32.lib")


CTcpClient::~CTcpClient(void)
{
}
void CTcpClient::HandleHeadRequest(int ret){
	if(myStatus == status_type::PACK_HD_INIT){

		if(ret == headLen){
			HandlePerfectHead();
		}
		else{
			myStatus = status_type::PACK_HD_RECVING;
			nowptr += ret;
			irecvlen -= ret;
		}
		
	}
	else if(myStatus == status_type::PACK_HD_RECVING){
		if(irecvlen == ret){
			HandlePerfectHead();
		}
		else{
			nowptr += ret;
			irecvlen -= ret;
		}
	}
	else if(myStatus == status_type::PACK_BD_INIT){
		if(ret == irecvlen){
			HandlePerfectBody();
		}
		else{
			myStatus = status_type::PACK_BD_RECVING;
			nowptr += ret;
			irecvlen -= ret;
		}

	}
	else if(myStatus == status_type::PACK_BD_RECVING){
		if(irecvlen == ret){
			HandlePerfectBody();
		}
		else{
			nowptr += ret;
			irecvlen -= ret;
		}

	}
	return;

}

void CTcpClient::HandlePerfectHead(void){
	LPCOMM_PKG_HEADER lpHeadData = (LPCOMM_PKG_HEADER)headData;
	unsigned short e_pkgLen = ntohs(lpHeadData->pkgLen);
	//恶意包或者错误包的判断
	if(e_pkgLen < headLen) 
	{
		//伪造包/或者包错误，否则整个包长怎么可能比包头还小？（整个包长是包头+包体，就算包体为0字节，那么至少e_pkgLen == m_iLenPkgHeader）
		//报文总长度 < 包头长度，认定非法用户，废包
		//状态和接收位置都复原，这些值都有必要，因为有可能在其他状态比如_PKG_HD_RECVING状态调用这个函数；
		myStatus  = status_type::PACK_BD_INIT;      
		nowptr = headData;
		irecvlen = headLen;
	}
	else if(e_pkgLen > (PKG_MAX_LENGTH))   //客户端发来包居然说包长度 > 29000?肯定是恶意包
	{
		//恶意包，太大，认定非法用户，废包【包头中说这个包总长度这么大，这不行】
		//状态和接收位置都复原，这些值都有必要，因为有可能在其他状态比如_PKG_HD_RECVING状态调用这个函数；
		myStatus  = status_type::PACK_BD_INIT;      
		nowptr = headData;
		irecvlen = headLen;
	}
	else{
		bodydata = new char[e_pkgLen];
		 memcpy(bodydata, headData, headLen);

		 if(e_pkgLen == headLen)
		 {
			 //该报文只有包头无包体【我们允许一个包只有包头，没有包体】
			 //这相当于收完整了，则直接入消息队列待后续业务逻辑线程去处理吧
			 HandlePerfectBody();
		 } 
		 else
		 {
			 //开始收包体，注意我的写法
			 myStatus = status_type::PACK_BD_INIT;
			 nowptr = bodydata + headLen; 
			 irecvlen = e_pkgLen - headLen;   
		 }                       
	}
}
void CTcpClient::HandlePerfectBody(void){



	myStatus = status_type::PACK_HD_INIT;
	nowptr = headData; 
	irecvlen = headLen;   

}