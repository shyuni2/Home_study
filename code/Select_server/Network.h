#pragma once
#include "NetUser.h"

class Network
{
public:
	SOCKET m_ListenSock;
public:
	bool IniNetwork();
	bool CloseNetwork();
	bool InitServer(int protocol, int iport, const char* ip=nullptr);
};

