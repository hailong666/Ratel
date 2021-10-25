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
	//��������ߴ�������ж�
	if(e_pkgLen < headLen) 
	{
		//α���/���߰����󣬷�������������ô���ܱȰ�ͷ��С�������������ǰ�ͷ+���壬�������Ϊ0�ֽڣ���ô����e_pkgLen == m_iLenPkgHeader��
		//�����ܳ��� < ��ͷ���ȣ��϶��Ƿ��û����ϰ�
		//״̬�ͽ���λ�ö���ԭ����Щֵ���б�Ҫ����Ϊ�п���������״̬����_PKG_HD_RECVING״̬�������������
		myStatus  = status_type::PACK_BD_INIT;      
		nowptr = headData;
		irecvlen = headLen;
	}
	else if(e_pkgLen > (PKG_MAX_LENGTH))   //�ͻ��˷�������Ȼ˵������ > 29000?�϶��Ƕ����
	{
		//�������̫���϶��Ƿ��û����ϰ�����ͷ��˵������ܳ�����ô���ⲻ�С�
		//״̬�ͽ���λ�ö���ԭ����Щֵ���б�Ҫ����Ϊ�п���������״̬����_PKG_HD_RECVING״̬�������������
		myStatus  = status_type::PACK_BD_INIT;      
		nowptr = headData;
		irecvlen = headLen;
	}
	else{
		bodydata = new char[e_pkgLen];
		 memcpy(bodydata, headData, headLen);

		 if(e_pkgLen == headLen)
		 {
			 //�ñ���ֻ�а�ͷ�ް��塾��������һ����ֻ�а�ͷ��û�а��塿
			 //���൱���������ˣ���ֱ������Ϣ���д�����ҵ���߼��߳�ȥ�����
			 HandlePerfectBody();
		 } 
		 else
		 {
			 //��ʼ�հ��壬ע���ҵ�д��
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