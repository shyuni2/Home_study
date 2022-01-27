#include "Network.h"

// 네트워크 연결 함수
bool Network::InitNetwork()
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
	m_Sock = socket(AF_INET, protocol, 0);
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
	int ret = bind(m_Sock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)  return false;
	ret = listen(m_Sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)  return false;
	return true;
}
bool Network::CloseNetwork()
{
	closesocket(m_Sock);	
	WSACleanup();
	return true;
}

// 데이터 주고받는 함수
int Network::SendData(SOCKET sock, char* msg, WORD type)
{
	// 1번 패킷 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2번 패킷 전송 
	char* pMsg = (char*)&packet;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pMsg[SendSize],
			packet.ph.len - SendSize, 0);
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
int Network::SendData(SOCKET sock, UPACKET& packet)
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
int Network::RecvData(NetUser& user)
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

// 유저추가
int Network::AddUser(SOCKET sock)
{
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	SOCKET clientSock = accept(sock, (sockaddr*)&clientAddr, &iLen);
	if (clientSock == SOCKET_ERROR)
	{
		return -1;
	}
	else
	{
		NetUser user;
		user.set(clientSock, clientAddr);
		userlist.push_back(user);
		cout << "ip =" << inet_ntoa(clientAddr.sin_addr) << "port =" << ntohs(clientAddr.sin_port) << " => 접속 " << endl;
		cout << "현재" << userlist.size() << " 명 접속중" << endl;
	}
	return 1;
}