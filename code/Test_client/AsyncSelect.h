#pragma once
#include <WinSock2.h>

#include "NetWork.h"
#define NETWORK_MSG WM_USER + 1

class AsyncSelect: public NetWork
{
public:
	bool m_Connect = false;
public:
	bool Connect(HWND hWnd, int protocol, int port, const char* ip);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

