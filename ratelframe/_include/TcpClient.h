/*
*  by CallMeEngineer  2021/10/24
*/

#pragma once

#include "commonDefine.h"
enum class status_type{
	PACK_HD_INIT,
	PACK_HD_RECVING,
	PACK_BD_INIT,
	PACK_BD_RECVING
};

class CTcpClient
{
public:
	CTcpClient(void){
		irecvlen = DATA_BUFSIZE;
		myStatus = status_type::PACK_HD_INIT;
		headLen = DATA_BUFSIZE;
		nowptr = headData;
	};
	~CTcpClient(void);
	int HandleHeadRequest(int ret);
	int HandlePerfectHead(void);
	int HandlePerfectBody(void);
	status_type myStatus;			//״̬��
	char headData[DATA_BUFSIZE];  //�洢ͷ��
	char* nowptr;					//��λ��ǰ�����λ��
	unsigned int irecvlen;
	unsigned int headLen;
	char* bodydata;
	
};

