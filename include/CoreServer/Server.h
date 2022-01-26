#pragma once
#include "NetUser.h"
class Server 
{
public:
	list<NetUser> m_UserList;
	SOCKET m_ListenSock;
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int Broadcast(NetUser* user);
	virtual bool AddUser(SOCKET sock, SOCKADDR_IN clientAddr);
public:
	virtual bool IniServer(int iPort);
	virtual bool Run();
	virtual bool Release();
};

