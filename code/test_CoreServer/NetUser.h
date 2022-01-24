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
	// ���۱�� �߰�
	char m_szRecvBuffer[2048];
	int  m_PacketPos; // ��Ŷ�� �����ּ�
	int  m_WritePos;  // ������ ���� ��ġ
	int  m_ReadPos;
	list<Packet> m_PacketPool;
public:
	int DispatchRead(char* RecvBuffer, int RecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

