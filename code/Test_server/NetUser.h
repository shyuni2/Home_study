#pragma once
#include <iostream>
#include <WinSock2.h>
#include <list>
#include <string>
#include "Packet.h"
#pragma comment (lib, "ws2_32.lib")

class NetUser
{
public:
	bool m_bConnect = false;
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short       m_iPort;
	// 버퍼기능 추가
	char m_szRecvBuffer[2048];
	int  m_iPacketPos; // 패킷의 시작주소
	int  m_iWritePos;  // 현재의 저장 위치
	int  m_iReadPos;
	list<Packet> m_PacketPool; // 패킷 넣기
public:
	int DispatchRead(char* szRecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

