#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "Packet.h"

#include <list>
#include <iostream>

using namespace std;

class NetUser
{
public:
	bool m_Connect = false; // �������
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_cAddr;
	short m_Port;
	char* m_name;
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

