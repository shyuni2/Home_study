#pragma once
#include "NetUser.h"

class Network
{
public:
	SOCKET m_Sock;
	list<NetUser> userlist;
	NetUser m_ChatUser;
public:
	bool InitNetwork();
	bool CloseNetwork();
	bool InitServer(int protocol, int iport, const char* ip=nullptr);
public:
	int SendData(SOCKET sock, UPACKET& packet);
	int AddUser(SOCKET sock);
	int RecvData(NetUser& user);
};

