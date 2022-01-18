#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#include <string>
#include "Packet.h"
#pragma comment	(lib, "ws2_32.lib")

class NetUser
{
public:
	bool m_bConnect = false; // 연결됐는지
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short m_iPort;
	// 버퍼기능 추가
	char m_szRecvBuffer[2048];
	int  m_iPacketPos; // 패킷의 시작주소
	int  m_iWritePos;  // 현재의 저장 위치
	int  m_iReadPos;
	list<Packet> m_PacketPool;
	int DispatchRead(char* szRecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

