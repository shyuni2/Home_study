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
	// ���۱�� �߰�
	char m_szRecvBuffer[2048];
	int  m_iPacketPos; // ��Ŷ�� �����ּ�
	int  m_iWritePos;  // ������ ���� ��ġ
	int  m_iReadPos;
	list<Packet> m_PacketPool; // ��Ŷ �ֱ�
public:
	int DispatchRead(char* szRecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

