#include "AsyncSelect.h"

bool AsyncSelect::Connect(HWND hWnd, int protocol, int iport, const char* ip)
{
	m_Sock = socket(AF_INET, protocol, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iport);
	sa.sin_addr.s_addr = inet_addr(ip);
	m_PlayerUser.m_Sock = m_Sock;

	if (WSAAsyncSelect(m_Sock, hWnd, NETWORK_MSG,
		FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		return false;
	}
	int iRet = WSAConnect(m_Sock, (sockaddr*)&sa, sizeof(sa), NULL, NULL, NULL, NULL);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}
LRESULT  AsyncSelect::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{	
	case NETWORK_MSG:
	{
		WORD wRet = WSAGETSELECTEVENT(lParam);
		switch (wRet)
		{
		case FD_CONNECT :
		{
			m_Connect = true;
		}break;
		case FD_CLOSE:
		{
			m_Connect = false;
		}break;
		case FD_READ:
		{
			RecvData(m_PlayerUser);
		}break;
		case FD_WRITE:
		{
			m_Connect = false;
		}break;
		}
	}break;
	}
	return 0;
}