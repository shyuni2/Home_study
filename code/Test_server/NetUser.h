#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iostream>
#include <list>
#include <string>
#include "Packet.h"

#pragma comment	(lib, "ws2_32.lib")

class NetUser
{
public:
	bool m_Connect = false;
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short m_iPort;
public: // ����
	char m_RecvBuffer[2048];
	int m_PacketPos; // ��Ŷ�� �����ּ�
	int m_WritePos;  // ������ ���� ��ġ
	int m_ReadPos;
public:
	list<Packet> m_PacketPool;
public:
	int DispatchRead(char* RecvBuffer, int RecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

