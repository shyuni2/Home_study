#pragma once
#include "NetUser.h"

class Server
{
public:
	list <NetUser> m_Userlist;
	CRITICAL_SECTION CS;
	SOCKET m_ListenSock;
public:
	int SendData(SOCKET sock, UPACKET& packet);
	int RecvData(NetUser& user);
	int BroadCast(NetUser& user);
public:
	bool InitServer(int port);
	bool Run();
	bool Release();
	virtual bool AddUser(SOCKET sock, SOCKADDR_IN CntAddr);
};

