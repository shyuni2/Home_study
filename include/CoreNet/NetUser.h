#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetStd.h"
#include "Packet.h"

class NetUser
{
public:
	bool m_bConnect = false;
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short m_iPort;
public: // ����
	char m_szRecvBuffer[2048];
	int  m_iPacketPos; // ��Ŷ�� �����ּ�
	int  m_iWritePos;  // ������ ���� ��ġ
	int  m_iReadPos;
	list<Packet> m_packetPool;
public:
	int DispatchRead(char* szRecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

