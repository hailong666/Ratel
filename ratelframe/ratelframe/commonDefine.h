#ifndef _COMMONDEFINE_H_
#define _COMMONDEFINE_H_

//定义存储头的大小
#define DATA_BUFSIZE       10
#define PKG_MAX_LENGTH     5000
#pragma pack(1)
typedef struct _COMM_PKG_HEADER{
	unsigned short pkgLen;	//包头+包体的总长度
	unsigned short msgCode;	//消息的协议或模式

}COMM_PKG_HEADER,*LPCOMM_PKG_HEADER;;



#pragma pack()
#endif
