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
	status_type myStatus;			//状态机
	char headData[DATA_BUFSIZE];  //存储头部
	char* nowptr;					//定位当前处理的位置
	unsigned int irecvlen;
	unsigned int headLen;
	char* bodydata;

};

