#pragma once
#include "Packet.h"
#include <list>

class NetUser
{
public:
	bool m_Connect = false; // �������
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short m_Port;
	// ���۱��
	char m_RecvBuffer[2048];
	int m_PacketPos;
	int m_WritePos;
	int m_ReadPos;
	list<Packet> m_PacketPool;
public:
	int DispatchRead(char* RecvBuffer, int RecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

