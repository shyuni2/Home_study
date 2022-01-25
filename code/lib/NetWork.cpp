#include "NetWork.h"



// 네트워크 설정
bool NetWork::InitServer(int port)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int iRet = bind(m_ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	iRet = listen(m_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	cout << "서버 실행중" << endl;
	u_long on = 1;
	ioctlsocket(m_ListenSock, FIONBIO, &on);

	return true;
}
bool NetWork::Run()
{
	SOCKET sock = m_ListenSock;
	SOCKADDR_IN ClientAddr;
	int Len = sizeof(ClientAddr);
	while (1)
	{
		SOCKET ClientSock = accept(sock, (sockaddr*)&ClientAddr, &Len);
		if (ClientSock == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "Error code = " << error << endl;
				break;
			}
		}
		else
		{
			AddUser(ClientSock, ClientAddr);
			//cout << "현재" << m_Userlist.size() << "명 접속중" << endl;
		}
	}
	return true;
}
bool NetWork::Release()
{
	closesocket(m_ListenSock);
	return true;
}
bool NetWork::AddUser(SOCKET sock, SOCKADDR_IN ClientAddr)
{
	return true;
}

