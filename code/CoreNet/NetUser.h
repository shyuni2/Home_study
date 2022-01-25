#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetStd.h" 
#include "Packet.h"

using namespace std;

class NetUser
{
public:
	char* m_name;
	bool m_Connect = false; // ����ƴ��� �Ǵ�
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short m_iPort;
	// ���۱�� �߰�
	char m_szRecvBuffer[2048];
	int m_PacketPos; //  ��Ŷ�� �����ּ�, ��Ŷ�� ����
	int m_WritePos; // ������ ���� ��ġ
	int m_ReadPos; // 
	list<Packet> m_PacketPool; // ��Ŷ �ֱ� (���Ḯ��Ʈ ����)
public:
	int DispatchRead(char* szRecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};
