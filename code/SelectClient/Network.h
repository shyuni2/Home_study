#pragma once
#include "NetUser.h"

class Network
{
public:
	SOCKET m_Sock;
	std::list<NetUser> userlist;
	NetUser m_PlayerUser;
public:
	bool InitNetwork();
	bool CloseNetwork();	
	bool InitServer(int protocol, int iport, const char* ip=nullptr);
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int AddUser(SOCKET sock);
	int RecvUser(NetUser& user);
};

