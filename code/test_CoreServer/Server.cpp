#include "Server.h"

int Server::SendData(SOCKET sock, UPACKET& packet)
{
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
int Server::RecvData(NetUser& user)
{
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(user.m_Sock, RecvBuffer, 1024, 0);
	if (RecvByte == 0)
	{
		return 0;
	}
	if (RecvByte == SOCKET_ERROR)
	{
		int Error = WSAGetLastError();
		if (Error != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return 2;
	}
	user.DispatchRead(RecvBuffer, RecvByte);
	return 1;
}
int Server::BroadCast(NetUser& user)
{
	if (user.m_PacketPool.size() > 0)
	{
		list<Packet>::iterator iter;
		for (iter = user.m_PacketPool.begin(); iter != user.m_PacketPool.end(); )
		{
			for (NetUser& senduser : m_Userlist)
			{
				int ret = SendData(senduser.m_Sock, (*iter).m_uPacket);
				if (ret <= 0)
				{
					senduser.m_bConnect = false;
				}
			}
			iter = user.m_PacketPool.erase(iter);
		}
	}
	return 1;
}
bool Server::AddUser(SOCKET sock, SOCKADDR_IN CntAddr)
{
	list<NetUser>::iterator useriter;

	return true;
}


bool Server::InitServer(int port)
{
	InitializeCriticalSection(&CS);
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
	int ret = bind(m_ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	ret = listen(m_ListenSock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		return false;
	}

	cout << "- 서버 가동중- " << endl;

	u_long on = 1;
	ioctlsocket(m_ListenSock, FIONBIO, &on);

	return true;
}
bool Server::Run()
{
	SOCKADDR_IN CntAddr;
	int iLen = sizeof(CntAddr);

	while (1)
	{
		SOCKET CntSock = accept(m_ListenSock, (sockaddr*)&CntAddr, &iLen);
		if (CntSock == SOCKET_ERROR)
		{
			int Error = WSAGetLastError();
			if (Error != WSAEWOULDBLOCK)
			{
				cout << "ErrorCode=" << Error << endl;
				break;
			}
		}
		else
		{
			NetUser user;
			user.set(CntSock, CntAddr);
			EnterCriticalSection(&CS);
			m_Userlist.push_back(user);
			LeaveCriticalSection(&CS);

			cout << "ip =" << inet_ntoa(CntAddr.sin_addr) << ", port =" << ntohs(CntAddr.sin_port) << " => 접속 " << endl;
			u_long on = 1;
			ioctlsocket(CntSock, FIONBIO, &on);
			cout << "현재 " << m_Userlist.size() << "명 접속중 " << endl;
		}
		Sleep(1);
	}
	//SetEvent();
	return true;
}
bool Server::Release()
{
	closesocket(m_ListenSock);
	WSACleanup();
	DeleteCriticalSection(&CS);

	return true;
}