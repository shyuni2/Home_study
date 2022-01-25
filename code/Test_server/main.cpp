#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetUser.h"

list<NetUser> m_Userlist;
CRITICAL_SECTION cs;


int SendData(SOCKET sock, char*msg, WORD type)
{
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg)+PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	char* pMsg = (char*)&packet;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pMsg[SendSize],
								packet.ph.len- SendSize, 0);
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
int SendData(SOCKET sock, UPACKET& packet)
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
int Broadcast(NetUser& user)
{
	if (user.m_PacketPool.size() > 0)
	{
		list<Packet>::iterator iter;
		for (iter = user.m_PacketPool.begin(); iter != user.m_PacketPool.end(); )
		{
			for (NetUser& senduser : m_Userlist)
			{
				int iRet = SendData(senduser.m_Sock, (*iter).m_uPacket);
				if (iRet <= 0)
				{
					senduser.m_Connect = false;
				}
			}
			iter = user.m_PacketPool.erase(iter);
		}
	}
	return 1;
}
int RecvUser(NetUser& user)
{
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(user.m_Sock, RecvBuffer, 1024, 0);
	if (RecvByte == 0)
	{
		return 0;
	}
	if (RecvByte == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK)
		{			
			return -1;
		}
		return 2;
	}
	user.DispatchRead(RecvBuffer, RecvByte);
	return 1;
}
DWORD WINAPI RecvThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		EnterCriticalSection(&cs);
		list<NetUser>::iterator userIter;
		for (userIter = m_Userlist.begin(); userIter != m_Userlist.end();)
		{
			int iRet = RecvUser(*userIter);
			if (iRet <= 0)
			{
				userIter = m_Userlist.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		LeaveCriticalSection(&cs);
		Sleep(1);
	}
}
DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		EnterCriticalSection(&cs);		
		list<NetUser>::iterator userIter;
		for (userIter = m_Userlist.begin(); userIter != m_Userlist.end();)
		{
			int iRet = Broadcast(*userIter);
			if (iRet <= 0)
			{
				userIter = m_Userlist.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		LeaveCriticalSection(&cs);
		Sleep(1);
	}
}
void main()
{
	InitializeCriticalSection(&cs);
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int iRet = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	iRet = listen(ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	cout	<< "서버 가동중......." << endl;

	u_long on = 1;
	ioctlsocket(ListenSock, FIONBIO, &on);

	DWORD ThreadId;
	HANDLE hThreadRecv = ::CreateThread(0, 0, RecvThread, (LPVOID)ListenSock, 0, &ThreadId);
	CloseHandle(hThreadRecv);
	DWORD ThreadIdSend;
	HANDLE hThreadSend = ::CreateThread(0, 0, SendThread, (LPVOID)ListenSock, 0, &ThreadIdSend);
	CloseHandle(hThreadSend);

	while (1)
	{
		SOCKET clientSock = accept(ListenSock, (sockaddr*)&clientAddr, &iLen);
		if (clientSock == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "ErrorCode=" << error << endl;
				break;
			}
		}
		else
		{
			NetUser user;
			user.set(clientSock, clientAddr);
			EnterCriticalSection(&cs);
			m_Userlist.push_back(user);
			LeaveCriticalSection(&cs);

			cout << "ip =" << inet_ntoa(clientAddr.sin_addr) << "port =" << ntohs(clientAddr.sin_port) << " => 접속 " << endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			cout << "현재" << m_Userlist.size() << " 명 접속중!" << endl;
		}
		Sleep(1);
	}
	closesocket(ListenSock);
	WSACleanup();

	DeleteCriticalSection(&cs);
}