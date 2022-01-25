#pragma once
#include "STD.h"
#include "Packet.h"
class NetWork
{
public:
	SOCKET m_ListenSock;
	SOCKADDR_IN m_Addr;
public: // 네트워크 설정
	virtual bool InitServer(int port);
	virtual bool Run();
	virtual bool Release();
	virtual bool AddUser(SOCKET sock, SOCKADDR_IN ClientAddr);
public:
	NetWork();
	~NetWork();
};

