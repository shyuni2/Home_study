#include "NetWork.h"

bool NetWork::InitNetWork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	return true;
}
bool NetWork::CloseNetWork()
{
	closesocket(m_Sock);
	WSACleanup();
	return true;
}
bool NetWork::InitServer(int protocol, int port, const char* ip)
{
	m_Sock = socket(AF_INET, protocol, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if (ip == nullptr)
	{
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		sa.sin_addr.s_addr = inet_addr(ip);
	}
	int ret = bind(m_Sock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	ret = listen(m_Sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

int NetWork::SendData(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pMsg[SendSize], packet.ph.len - SendSize, 0);
		if (SendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		SendSize += SendByte;
	} while (SendSize < packet.ph.len);
	return SendSize;
}
int NetWork::RecvData(NetUser& user)
{
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(user.m_Sock, RecvBuffer, 1024, 0);
	if (RecvByte == 0)
	{
		return 0;
	}
	if (RecvByte == SOCKET_ERROR)
	{
		return -1;
	}
	user.DispatchRead(RecvBuffer, RecvByte);
	return 1;
}
int NetWork::AddUser(SOCKET sock)
{
	SOCKADDR_IN ClientAddr;
	int caLength = sizeof(ClientAddr);
	SOCKET ClientSock = accept(sock, (sockaddr*)&ClientAddr, &caLength);
	if (ClientSock == SOCKET_ERROR)
	{
		return -1;
	}
	else
	{
		NetUser user;
		user.set(ClientSock, ClientAddr);
		userlist.push_back(user);
		cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << " port =" << ntohs(ClientAddr.sin_port) << "  " << endl;
		cout << "-" << userlist.size() << " 명 접속중 -" << endl;
	}
	return 1;
}