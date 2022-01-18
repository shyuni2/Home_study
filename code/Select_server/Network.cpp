#include "Network.h"

bool Network::IniNetwork()
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	return true;
}
bool Network::InitServer(int protocol, int iport, const char* ip)
{
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iport);
	if (ip == nullptr)
	{
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		sa.sin_addr.s_addr = inet_addr(ip);
	}
	int iRet = bind(m_ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return false;
	iRet = listen(m_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return false;
	return true;
}
bool Network::CloseNetwork()
{
	closesocket(m_ListenSock);	
	WSACleanup();
	return true;
}