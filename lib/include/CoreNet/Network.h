#pragma once
#include "NetUser.h"

// �������� �����ѰŶ� ������� ���� 
class Network
{
public:
	SOCKET m_Sock;
	list<NetUser> userlist;
	NetUser m_PlayerUser;
public:
	bool InitNetwork();
	bool InitServer(int protocol, int iport, const char* ip = nullptr);
	bool CloseNetwork();
public:
	// Ŭ��ϱ� �����߰� �Լ��� �ʿ�x
	int SendData(SOCKET sock, char* msg, WORD type);
	int SendData(SOCKET sock, UPACKET& packet);
	int RecvData(NetUser& user);
};

