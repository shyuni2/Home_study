#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "STDserver.h"
#include "Packet.h"

class NetUser
{
public:
	bool m_bConnect = false;
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short m_iPort;
	// 버퍼기능 추가
	char m_szRecvBuffer[2048];
	int  m_PacketPos; // 패킷의 시작주소
	int  m_WritePos;  // 현재의 저장 위치
	int  m_ReadPos;
	list<Packet> m_PacketPool;
public:
	int DispatchRead(char* RecvBuffer, int RecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

