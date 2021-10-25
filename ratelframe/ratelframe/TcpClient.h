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
	CTcpClient(void){};
	~CTcpClient(void);
	void HandleHeadRequest(int ret);
	void HandlePerfectHead(void);
	void HandlePerfectBody(void);
	status_type myStatus;			//״̬��
	char headData[DATA_BUFSIZE];  //�洢ͷ��
	char* nowptr;					//��λ��ǰ�����λ��
	unsigned int irecvlen;
	unsigned int headLen;
	char* bodydata;

};

