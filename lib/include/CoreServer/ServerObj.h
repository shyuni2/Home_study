#pragma once
#include "ServerStd.h"

class ServerObj
{
public:
	CRITICAL_SECTION m_CS;
	HANDLE m_hKillEvent;
public:
	ServerObj();
	virtual ~ServerObj();
};

