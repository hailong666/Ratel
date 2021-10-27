/*
*  by CallMeEngineer  2021/10/24
*/

#include "stdafx.h"
#include "MySocket.h"

CTcpClient CMySocket::tcpClient;
CThreadMangerPool CMySocket::threadPool;
CMySocket::CMySocket(void):ifClose(true)
{

}
CMySocket::~CMySocket(void)
{
	closesocket(serverSocket);
	WSACleanup();
}
bool CMySocket::initSocket(){
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup() is error!\n";
		return false;
	}
	CMySocket::threadPool.init(5);
	return true;
}
bool CMySocket::bindSocket(int listenport){
	std::string addr = "0.0.0.0";
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		return false;
	}
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	inet_pton(AF_INET, addr.data(), &servAdr.sin_addr.s_addr);
	servAdr.sin_port = htons(listenport);

	//Ԥ�����÷���������I/O
	//if (ifSetNonBlock) {
	//	///����socketΪ������socket
	//	u_long argp = 1;
	//	int ret = ioctlsocket(serverSocket, FIONBIO, &argp);
	//	if (ret != NO_ERROR) {
	//		std::cerr << "ioctlsocket error! : " << WSAGetLastError() << std::endl;
	//		return false;
	//	}
	//}
	if(bind(serverSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR){
		std::cerr << "�󶨶˿ڳ���" << std::endl;
	}
	if(listen(serverSocket, 5) == SOCKET_ERROR){
		std::cerr << "�����˿ڳ���" << std::endl;
	}
	clientfds.push_back(serverSocket);
	return true;
}
void CMySocket::Run() {

	while (ifClose) {
		fd_set readset;
		//fd_set writeset;
		//FD_ZERO(&writeset);		
		FD_ZERO(&readset);
		//timeval tm;
		int fdmax = 0;
		timeval tm;
		tm.tv_sec = 3;
		tm.tv_usec = 0;
		for(unsigned int i = 0; i < clientfds.size(); i++){

			FD_SET(clientfds[i], &readset);
			//FD_SET(serverSocke+t, &writeset);
			if(fdmax < clientfds[i])
				fdmax = clientfds[i];

		}
		int ret = select(fdmax + 1, &readset, NULL, NULL, &tm);

		if (ret == -1) {

			std::cerr << "select error!\n";
			break;

		}
		else if (ret == 0) { //select������ʱ     
			//std::cout << "no event in specific time interval." << std::endl;
			continue;
		}
		else
		{
			//���ɶ��¼�
			if (FD_ISSET(serverSocket, &readset)) {
				SOCKADDR_IN client;
				SOCKET accpClient;
				int clientlength = sizeof(client);
				accpClient = accept(serverSocket,(SOCKADDR*)&client,&clientlength);
				clientfds.push_back(accpClient);
				std::cout << " �����ӵ�����" << std::endl;
				LOGD("%d �ͻ������ӳɹ�", accpClient );

			}
			else {
				for(unsigned int i = 0; i < clientfds.size(); i++ ){
					
					if(clientfds[i] != -1 && FD_ISSET(clientfds[i], &readset)){
						int ret = -1;
						if(CMySocket::tcpClient.myStatus == status_type::PACK_HD_INIT||CMySocket::tcpClient.myStatus == status_type::PACK_HD_RECVING)
							ret = HandleRecv(clientfds[i], CMySocket::tcpClient.headData, CMySocket::tcpClient.irecvlen, i);
						else
							ret = HandleRecv(clientfds[i], CMySocket::tcpClient.bodydata + DATA_BUFSIZE, CMySocket::tcpClient.irecvlen, i);

						if(ret == -1){
							continue;
						}
						int bodyLen = CMySocket::tcpClient.HandleHeadRequest(ret);
						if(bodyLen != -1){
							
							threadPool.addTask(new ThreadTask(CMySocket::tcpClient.bodydata, bodyLen));
						}
							
					}else
					{
							//�����������������¼�
					}

				}

			}
		}

	}
}
int CMySocket::HandleRecv(int fd, char* buf, unsigned int buflen,int i){
	
	memset(buf, 0 ,buflen);
	int len = recv(fd, buf, buflen, 0);

	if(len < 0){
		LOGE("���յ������������ݰ�����Ϊ����");
		std::cerr << " �������ݳ���" << std::endl;
		clientfds.erase(clientfds.begin() + i);
		closesocket(clientfds[i]);
	}
	else if(len == 0){
		LOGE("�ͻ��� %d �ر�����!",clientfds[i]);
		std::cerr << " �Է��ر����ӣ�" << std::endl;
		clientfds.erase(clientfds.begin() + i);
		closesocket(clientfds[i]);

	}else{
		return len;
		//��������
	}
	return -1;
}