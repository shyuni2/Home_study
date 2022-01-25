#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "AsyncSelect.h"

bool AsyncSelect::Connect(HWND hWnd, int protocol, int port, const char* ip)
{
	m_Sock = socket(AF_INET, protocol, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);
	m_Chatuser.m_Sock = m_Sock;

	if (WSAAsyncSelect(m_Sock, hWnd, NETWORK_MSG, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		return false;
	}
	int ret = WSAConnect(m_Sock, (sockaddr*)&sa, sizeof(sa), NULL, NULL, NULL, NULL);
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}
LRESULT AsyncSelect::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case NETWORK_MSG:
	{
		WORD wRet = WSAGETSELECTEVENT(lParam);
		switch (wRet)
		{
		case FD_CONNECT:
		{
			char m_name[20];
			cout << "ÀÌ¸§ÀÔ·Â : ";
			cin >> m_name;
			memcpy(m_Chatuser.m_name, m_name, sizeof(m_name));
			cout << m_Chatuser.m_name << "´ÔÀÌ ÀÔÀåÇÏ¼Ì½À´Ï´Ù" << endl;

			m_Connect = true;
		}break;
		case FD_CLOSE:
		{
			cout << m_Chatuser.m_name << "´ÔÀÌ ÅðÀåÇÏ¼Ì½À´Ï´Ù" << endl;
			m_Connect = false;
		}break;
		case FD_READ:
		{
			RecvData(m_Chatuser);
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