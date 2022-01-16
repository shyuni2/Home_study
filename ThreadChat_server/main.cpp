#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetUser.h"

list<NetUser> m_UserList;
CRITICAL_SECTION CS;

int SendData(SOCKET sock, UPACKET& packet)
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
int BroadCast(NetUser& user)
{
	if (user.m_PacketPool.size() > 0)
	{
		list<Packet>::iterator iter;
		for (iter = user.m_PacketPool.begin(); iter != user.m_PacketPool.end();)
		{
			for (NetUser& senduser : m_UserList)
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
int RecvData(NetUser& user)
{
	char szRecvBuffer[1024] = { 0, };
	int RecvByte = recv(user.m_Sock, szRecvBuffer, 1024, 0);
	if (RecvByte == 0) return 0;
	if (RecvByte == SOCKET_ERROR)
	{
		int Error = WSAGetLastError();
		if (Error != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return 2;
	}
	user.DispatchRead(szRecvBuffer, RecvByte);
	return 1;
}
// 스레드
DWORD WINAPI RecvThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		EnterCriticalSection(&CS); // 크리티컬 섹션 구간 시작
		list<NetUser>::iterator useriter;
		for (useriter = m_UserList.begin(); useriter != m_UserList.end();)
		{
			int ret = RecvData(*useriter);
			if (ret <= 0)
			{
				useriter = m_UserList.erase(useriter);
			}
			else
			{
				useriter++;
			}
		}
		LeaveCriticalSection(&CS); // 크리티컬 섹션 구간 끝
		Sleep(1); // 스위칭
	}
}
DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		EnterCriticalSection(&CS); // 크리티컬 섹션 구간 시작
		list<NetUser>::iterator useriter;
		for (useriter = m_UserList.begin(); useriter != m_UserList.end();)
		{
			int ret = BroadCast(*useriter);
			if (ret <= 0)
			{
				useriter = m_UserList.erase(useriter);
			}
			else
			{
				useriter++;
			}
		}
		LeaveCriticalSection(&CS); // 크리티컬 섹션 구간 끝
		Sleep(1); // 스위칭
	}
}

// 메인함수
void main()
{
	InitializeCriticalSection(&CS); // 크리티컬 섹션 초기화

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
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

	SOCKADDR_IN ClientAddr;
	int CALength = sizeof(ClientAddr);

	cout << " - 서버 가동중 - " << endl;

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
		SOCKET ClientSock = accept(ListenSock, (sockaddr*)&ClientAddr, &CALength);
		if (ClientSock == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				cout << "ErrorCode=" << iError << endl;
				break;
			}
		}
		else
		{
			NetUser user;
			user.set(ClientSock, ClientAddr);
			EnterCriticalSection(&CS);
			m_UserList.push_back(user);
			LeaveCriticalSection(&CS);

			cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << "port =" << ntohs(ClientAddr.sin_port) << " -> 접속" << endl;
			u_long on = 1;
			ioctlsocket(ClientSock, FIONBIO, &on);
			cout <<"- "<< m_UserList.size() << " 명 접속중 -" << endl;
		}
		Sleep(1);
	}
	closesocket(ListenSock);
	WSACleanup();

	DeleteCriticalSection(&CS); // 크리티컬 섹션 종료
}
