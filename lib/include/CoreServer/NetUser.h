#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ServerObj.h"
#include "Packet.h"

struct TOV // ����������ü
{
	OVERLAPPED ov; // �ؿ� ������ ĳ���� �ȵ�
	int type;
};

class NetUser 
{
public:
	bool m_bConnect = false;
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_Name;
	short m_iPort;
	// ���۱�� �߰�
	TOV m_ovRecv;
	TOV m_ovSend;
	WSABUF m_wsaRecvBuffer;
	WSABUF m_wsaSendBuffer;
	char m_szRecv[256];
	char m_szSend[256];
public:
	char m_szRecvBuffer[2048];
	int m_iPacketPos; // ��Ŷ�� �����ּ�
	int m_iWritePos;  // ������ ���� ��ġ
	int m_iReadPos;
	list<Packet> m_PacketPool;
public:
	int Dispatch(DWORD dwTrans, TOV* tov);
	int DispatchRecv(char* szRecvBuffer, int iRecvByte);
	int DispatchSend(DWORD dwTrans);
	void set(SOCKET sock, SOCKADDR_IN addr);
	int Recv();
	bool DisConnect();
public:
	NetUser();
	virtual ~NetUser();
};

