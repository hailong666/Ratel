
// MFCApplication1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
//#include "ThreadMangerPool.h"
//#include "ngx_c_crc32.h"
#include <iostream>
#include <vector>
#include <sstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
std::vector<SOCKET> clients;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg �Ի���



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication1Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication1Dlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CMFCApplication1Dlg ��Ϣ�������

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




SOCKET            sClient; //��ȫ������
SOCKET            client; //��ȫ������
//���� ���� ��ť ����ñ�����
void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox(_T("aaaa"));
	//����
	static int ifinit = false;
	if (ifinit == false)
	{
		ifinit = true;
		WSADATA m_wsadata;
		if (WSAStartup(0x0202, &m_wsadata))  //��windowsƽ̨ʹ��socketǰ�����Ǳ���Ҫ����һ�³�ʼ��,����Ӧ����WSACleanup()�ͷţ�����Ϊ�ǲ��Դ��룬������ν��
		{
			AfxMessageBox(_T("socket��ʼ��ʧ��!"));
		}
	}

	//(1)����socket
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		AfxMessageBox(_T("socket()ʧ��"));
		return;
	}
	//(2)���ӷ�����
	

	
	SOCKADDR_IN       server_in;
	memset(&server_in, 0, sizeof(SOCKADDR_IN));
	server_in.sin_family = AF_INET;
	server_in.sin_port = htons(80);
	server_in.sin_addr.s_addr = inet_addr("127.0.0.1");

	//CThreadMangerPool thread;
	/*thread.init(10);
	for(int i = 3001; i < 5000; i++){
		ThreadTask* task = new ThreadTask(i);
		thread.addTask(task);
	}*/

	/*SOCKADDR_IN       client_in;
	memset(&client_in, 0, sizeof(SOCKADDR_IN));
	client_in.sin_family = AF_INET;
	client_in.sin_port = htons(3000);
	client_in.sin_addr.s_addr = inet_addr("127.0.0.1");*/

	//if(bind(sClient, (SOCKADDR*)&client_in, sizeof(client_in)) == SOCKET_ERROR){
	//	
	//	std::cerr << "�󶨶˿ڳ���" << std::endl;
	//}

	if (connect(sClient, (struct sockaddr *)&server_in, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("connect()ʧ��"));
		closesocket(sClient);
		return;
	}
	
	//(3)һ�������������ø��շ����ݳ�ʱʱ�䣬����send(),recv()��������ʱ����ȫ����
	int iSendRecvTimeOut = 5000;
	if (setsockopt(sClient, SOL_SOCKET, SO_SNDTIMEO, (const char *)&iSendRecvTimeOut, sizeof(int)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("setsockopt(SO_SNDTIMEO)ʧ��"));
		closesocket(sClient);
		return;
	}
	if (setsockopt(sClient, SOL_SOCKET, SO_RCVTIMEO, (const char *)&iSendRecvTimeOut, sizeof(int)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("setsockopt(SO_RCVTIMEO)ʧ��"));
		closesocket(sClient);
		return;
	}
	
	AfxMessageBox(_T("�ǳ��ã�connect()�ɹ�"));
	//}
	return;
}

//�������ݣ�ֵ�ý���
int SendData(SOCKET sSocket, char *p_sendbuf, int ibuflen)
{
	int usend = ibuflen; //Ҫ���͵���Ŀ
	int uwrote = 0;      //�ѷ��͵���Ŀ
	int tmp_sret;

	while (uwrote < usend)
	{
		tmp_sret = send(sSocket, p_sendbuf + uwrote, usend - uwrote, 0);
		if ((tmp_sret == SOCKET_ERROR) || (tmp_sret == 0))
		{
			//�д�������
			return SOCKET_ERROR;
		}
		uwrote += tmp_sret;
	}//end while
	return uwrote;
}


//�ṹ����------------------------------------
#pragma pack (1) //���뷽ʽ,1�ֽڶ��� 
//һЩ������ͨѶ��صĽṹ��������
typedef struct _COMM_PKG_HEADER
{
	unsigned short pkgLen;    //�����ܳ��ȡ���ͷ+���塿--2�ֽڣ�2�ֽڿ��Ա�ʾ���������Ϊ6��࣬���Ƕ���_PKG_MAX_LENGTH 30000��������pkgLen�㹻������
	unsigned short msgCode;   //��Ϣ���ʹ���--2�ֽڣ���������ÿ����ͬ�������ͬ����Ϣ��
	char           msgUnUseful[6];
	//int            crc32;     //CRC32Ч��--4�ֽڣ�Ϊ�˷�ֹ�շ������г����յ����ݺͷ������ݲ�һ�µ��������������ֶ���һ��������У����	
}COMM_PKG_HEADER, *LPCOMM_PKG_HEADER;

typedef struct _STRUCT_REGISTER
{
	int           iType;          //����
	char          username[56];   //�û��� 
	char          password[40];   //����

}STRUCT_REGISTER, *LPSTRUCT_REGISTER;


typedef struct _STRUCT_LOGIN
{
	char          username[56];   //�û��� 
	char          password[40];   //����

}STRUCT_LOGIN, *LPSTRUCT_LOGIN;

typedef struct _STRUCT_HTTP
{
	int           iType; 
	char		  httphead[96];

}STRUCT_HTTP,*LPSTRUCT_HTTP;
#pragma pack() //ȡ��ָ�����룬�ָ�ȱʡ����

int  g_iLenPkgHeader = sizeof(COMM_PKG_HEADER);

//���� ���� ��ť ����ñ�����
void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox(_T("bbbb"));
	//�ȷ���ע������
	//CCRC32   *p_crc32 = CCRC32::GetInstance();
	int test = g_iLenPkgHeader + sizeof(STRUCT_REGISTER);
	char *p_sendbuf = (char *)new char[g_iLenPkgHeader + sizeof(STRUCT_REGISTER)];

	LPCOMM_PKG_HEADER         pinfohead;
	pinfohead = (LPCOMM_PKG_HEADER)p_sendbuf;
	pinfohead->msgCode = 5;
	pinfohead->msgCode = htons(pinfohead->msgCode);
	//pinfohead->crc32 = htonl(123); //���ԣ���������� һ��
	pinfohead->pkgLen = htons(g_iLenPkgHeader + sizeof(STRUCT_REGISTER));

	LPSTRUCT_REGISTER pstruc_sendstruc = (LPSTRUCT_REGISTER)(p_sendbuf + g_iLenPkgHeader);
	pstruc_sendstruc->iType = htonl(80);
	strcpy(pstruc_sendstruc->username, "MyNameIsZhangSan");
	strcpy(pstruc_sendstruc->password, "123456789");
	//crcֵ��Ҫ������
	//pinfohead->crc32   = p_crc32->Get_CRC((unsigned char *)pstruc_sendstruc, sizeof(STRUCT_REGISTER));
	//pinfohead->crc32   = htonl(pinfohead->crc32); //������ֽ����֣�������ת������

	if (SendData(sClient, p_sendbuf, g_iLenPkgHeader + sizeof(STRUCT_REGISTER)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("SendData()ʧ��"));
		//���Գ���Ҳ�Ͳ��ر�socket��ֱ�ӷ���ȥ����
		delete[] p_sendbuf; //�ڴ��ܻ���Ҫ�ͷŵ�
		return; 
	}

	delete[] p_sendbuf; //�ͷ��ϱߵ��ڴ�

	//�ٷ�����¼����
	p_sendbuf = (char *)new char[g_iLenPkgHeader + sizeof(STRUCT_LOGIN)];

	//LPCOMM_PKG_HEADER         pinfohead;
	pinfohead = (LPCOMM_PKG_HEADER)p_sendbuf;
	pinfohead->msgCode = 6;  //�仯һ��
	pinfohead->msgCode = htons(pinfohead->msgCode);
	//pinfohead->crc32 = htonl(345); //���ԣ���������� һ��
	pinfohead->pkgLen = htons(g_iLenPkgHeader + sizeof(STRUCT_LOGIN));

	LPSTRUCT_LOGIN pstruc_sendstruc2 = (LPSTRUCT_LOGIN)(p_sendbuf + g_iLenPkgHeader);
	strcpy(pstruc_sendstruc2->username, "5678");

	//crcֵ��Ҫ������
	//pinfohead->crc32 = p_crc32->Get_CRC((unsigned char *)pstruc_sendstruc2, sizeof(STRUCT_LOGIN));
	//pinfohead->crc32 = htonl(pinfohead->crc32); //������ֽ����֣�������ת������

	//if (SendData(sClient, p_sendbuf, g_iLenPkgHeader + sizeof(STRUCT_LOGIN)) == SOCKET_ERROR)
	//{
	//	AfxMessageBox(_T("SendData()ʧ��"));
	//	//���Գ���Ҳ�Ͳ��ر�socket��ֱ�ӷ���ȥ����
	//	delete[] p_sendbuf; //�ڴ��ܻ���Ҫ�ͷŵ�
	//	return;
	//}
	//delete[] p_sendbuf;
	//closesocket(sClient);

	AfxMessageBox(_T("�ǳ��ã��������ݰ�ͨ��SendData()���������"));
}


void CMFCApplication1Dlg::OnBnClickedButton3()
{
	closesocket(client);
	//WSACleanup();
	AfxMessageBox(_T("�ɹ��Ͽ����ӣ�"));

}


void CMFCApplication1Dlg::OnBnClickedButton5()
{
	CString cStr;
	GetDlgItem(IDC_EDIT1)->GetWindowText(cStr);
	int x = 0;
	/*cStr.Format(_T("%d"),x );*/
	x = _ttoi(cStr);
	if(x<=0){
		AfxMessageBox(_T("��������������"));
		return ;
	}
	for( int i = 0; i < x ; i++){
		client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (client == INVALID_SOCKET)
		{
			AfxMessageBox(_T("socket()ʧ��"));
			continue;

		}
		SOCKADDR_IN       server_in;
		memset(&server_in, 0, sizeof(SOCKADDR_IN));
		server_in.sin_family = AF_INET;
		server_in.sin_port = htons(80);
		server_in.sin_addr.s_addr = inet_addr("127.0.0.1");
		if (connect(client, (struct sockaddr *)&server_in, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		{
			//AfxMessageBox(_T("connect()ʧ��"));
			closesocket(client);
			return;
		}
		int iSendRecvTimeOut = 5000;
		if (setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (const char *)&iSendRecvTimeOut, sizeof(int)) == SOCKET_ERROR)
		{
			//AfxMessageBox(_T("setsockopt(SO_SNDTIMEO)ʧ��"));
			closesocket(client);
			return;
		}
		if (setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char *)&iSendRecvTimeOut, sizeof(int)) == SOCKET_ERROR)
		{
			//AfxMessageBox(_T("setsockopt(SO_RCVTIMEO)ʧ��"));
			closesocket(client);
			return;
		}
		clients.push_back(client);
	}
	std::string strs;
	std::ostringstream buffer;
	buffer << "�ǳ��ã�һ���� " << clients.size() << " �����ӳɹ�!" << std::endl;
	strs = buffer.str();
	CString ccstr(strs.data());
	AfxMessageBox(ccstr);

}


void CMFCApplication1Dlg::OnBnClickedButton6()
{
	for(int i = 0; i < clients.size(); i++){
		closesocket(clients[i]);
	}
	clients.clear();

	//WSACleanup();
	AfxMessageBox(_T("�Ѿ��ɹ��Ͽ��������ӣ�"));
}


void CMFCApplication1Dlg::OnBnClickedButton4()
{
	int test = g_iLenPkgHeader + sizeof(STRUCT_REGISTER);
	char *p_sendbuf = (char *)new char[g_iLenPkgHeader + sizeof(STRUCT_REGISTER)];

	LPCOMM_PKG_HEADER         pinfohead;
	pinfohead = (LPCOMM_PKG_HEADER)p_sendbuf;
	pinfohead->msgCode = 5;
	pinfohead->msgCode = htons(pinfohead->msgCode);
	//pinfohead->crc32 = htonl(123); //���ԣ���������� һ��
	pinfohead->pkgLen = htons(g_iLenPkgHeader + sizeof(STRUCT_REGISTER));

	LPSTRUCT_REGISTER pstruc_sendstruc = (LPSTRUCT_REGISTER)(p_sendbuf + g_iLenPkgHeader);
	pstruc_sendstruc->iType = htonl(80);
	strcpy(pstruc_sendstruc->username, "MyNameIsZhangSan");
	strcpy(pstruc_sendstruc->password, "123456789");
	//crcֵ��Ҫ������
	//pinfohead->crc32   = p_crc32->Get_CRC((unsigned char *)pstruc_sendstruc, sizeof(STRUCT_REGISTER));
	//pinfohead->crc32   = htonl(pinfohead->crc32); //������ֽ����֣�������ת������

	CString cStr;
	GetDlgItem(IDC_EDIT2)->GetWindowText(cStr);

	int x = 0;
	/*cStr.Format(_T("%d"),x );*/
	x = _ttoi(cStr);
	if(x<=0){
		AfxMessageBox(_T("������������"));
		return ;
	}

	for(int i = 0; i < x ;i++){
		if (SendData(sClient, p_sendbuf, g_iLenPkgHeader + sizeof(STRUCT_REGISTER)) == SOCKET_ERROR)
		{
			//AfxMessageBox(_T("SendData()ʧ��"));
			//���Գ���Ҳ�Ͳ��ر�socket��ֱ�ӷ���ȥ����
			delete[] p_sendbuf; //�ڴ��ܻ���Ҫ�ͷŵ�
			return; 
		}
	}


	delete[] p_sendbuf; //�ͷ��ϱߵ��ڴ�
}


void CMFCApplication1Dlg::OnBnClickedButton7()
{
	int test = g_iLenPkgHeader + sizeof(STRUCT_HTTP);
	char *p_sendbuf = (char *)new char[g_iLenPkgHeader + sizeof(STRUCT_HTTP)];

	LPCOMM_PKG_HEADER         pinfohead;
	pinfohead = (LPCOMM_PKG_HEADER)p_sendbuf;
	pinfohead->msgCode = 5;
	pinfohead->msgCode = htons(pinfohead->msgCode);
	//pinfohead->crc32 = htonl(123); //���ԣ���������� һ��
	pinfohead->pkgLen = htons(g_iLenPkgHeader + sizeof(STRUCT_HTTP));

	LPSTRUCT_HTTP pstruc_sendstruc = (LPSTRUCT_HTTP)(p_sendbuf + g_iLenPkgHeader);
	pstruc_sendstruc->iType = htonl(90);
	strcpy(pstruc_sendstruc->httphead, "GET / HTTP/1.1\r\nHost:img.mukewang.com\r\nAccept:image/webp,image\r\n\r\n");
	if (SendData(sClient, p_sendbuf, g_iLenPkgHeader + sizeof(STRUCT_HTTP)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("SendData()ʧ��"));
		//���Գ���Ҳ�Ͳ��ر�socket��ֱ�ӷ���ȥ����
		delete[] p_sendbuf; //�ڴ��ܻ���Ҫ�ͷŵ�
		return; 
	}
}


void CMFCApplication1Dlg::OnBnClickedButton8()
{
	int test = g_iLenPkgHeader + sizeof(STRUCT_HTTP);
	char *p_sendbuf = (char *)new char[g_iLenPkgHeader + sizeof(STRUCT_HTTP)];

	LPCOMM_PKG_HEADER         pinfohead;
	pinfohead = (LPCOMM_PKG_HEADER)p_sendbuf;
	pinfohead->msgCode = 5;
	pinfohead->msgCode = htons(pinfohead->msgCode);
	//pinfohead->crc32 = htonl(123); //���ԣ���������� һ��
	pinfohead->pkgLen = htons(g_iLenPkgHeader + sizeof(STRUCT_HTTP));

	LPSTRUCT_HTTP pstruc_sendstruc = (LPSTRUCT_HTTP)(p_sendbuf + g_iLenPkgHeader);
	pstruc_sendstruc->iType = htonl(90);
	strcpy(pstruc_sendstruc->httphead, "POST / HTTP/1.1\r\nHost:img.mukewang.com\r\nAccept:image/webp,image\r\n\r\nname=Ajax&publisher=Wiley");
	if (SendData(sClient, p_sendbuf, g_iLenPkgHeader + sizeof(STRUCT_HTTP)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("SendData()ʧ��"));
		//���Գ���Ҳ�Ͳ��ر�socket��ֱ�ӷ���ȥ����
		delete[] p_sendbuf; //�ڴ��ܻ���Ҫ�ͷŵ�
		return; 
	}
}
