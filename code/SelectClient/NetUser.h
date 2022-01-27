#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Packet.h"

class NetUser
{
public:
	bool m_Connect = false;
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short m_Port;
public: // ����
	char m_RecvBuffer[2048];
	int m_PacketPos; // ��Ŷ�� �����ּ�
	int m_WritePos;  // ������ ���� ��ġ
	int m_ReadPos;
public:
	list<Packet> m_PacketPool;
public:
	int DispatchRead(char* RecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

