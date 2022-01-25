#pragma once
#include "NetUser.h"

class NetWork
{
public:
	SOCKET m_Sock;
	list<NetUser> userlist;
	NetUser m_Chatuser;
public:
	bool InitNetWork();
	bool CloseNetWork();
	bool InitServer(int protocol, int port, const char* ip);
public:
	int SendData(SOCKET sock, UPACKET& packet);
	int RecvData(NetUser& user);
	int AddUser(SOCKET sock);
};

