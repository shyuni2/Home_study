#pragma once
#include "NetUser.h"

class Network
{
public:
	SOCKET m_Sock;	
	NetUser m_ChatUser;
public:
	list<NetUser> userlist;
public:
	bool InitNetwork();
	bool CloseNetwork();	
	bool InitServer(int protocol, int iport, const char* ip = nullptr);
public:
	int SendData(SOCKET sock, char* msg, WORD type);
	int SendData(SOCKET sock, UPACKET& packet);
	int AddUser(SOCKET sock);
	int RecvData(NetUser& user);
	int Broadcast(NetUser& user);
};

