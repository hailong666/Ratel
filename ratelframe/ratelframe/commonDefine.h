#ifndef _COMMONDEFINE_H_
#define _COMMONDEFINE_H_

//����洢ͷ�Ĵ�С
#define DATA_BUFSIZE       10
#define PKG_MAX_LENGTH     5000
#pragma pack(1)
typedef struct _COMM_PKG_HEADER{
	unsigned short pkgLen;	//��ͷ+������ܳ���
	unsigned short msgCode;	//��Ϣ��Э���ģʽ

}COMM_PKG_HEADER,*LPCOMM_PKG_HEADER;;



#pragma pack()
#endif
