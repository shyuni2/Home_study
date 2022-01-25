#include "Server.h"



DWORD WINAPI RecvThread(LPVOID param)
{
	Server* pServer = (Server*)param;
	SOCKET sock = pServer->m_ListenSock;
	while (1)
	{
		list<NetUser>::iterator userIter;
		for (userIter = pServer->m_Userlist.begin(); userIter != pServer->m_Userlist.end();)
		{
			int iRet = pServer->RecvData(*userIter);
			if (iRet <= 0)
			{
				userIter = pServer->m_Userlist.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		Sleep(1);
	}
}

DWORD WINAPI SendThread(LPVOID param)
{
	Server* pServer = (Server*)param;
	SOCKET sock = pServer->m_ListenSock;
	while (1)
	{
		list<NetUser>::iterator userIter;
		for (userIter = pServer->m_Userlist.begin(); userIter != pServer->m_Userlist.end();)
		{
			int iRet = pServer->BroadCast(*userIter);
			if (iRet <= 0)
			{
				userIter = pServer->m_Userlist.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		Sleep(1);
	}
}

void main()
{
	Server server;
	server.InitServer(1);
	DWORD ThreadIdRecv;
	HANDLE hThreadRecv = ::CreateThread(0, 0, RecvThread, (LPVOID)&server, 0, &ThreadIdRecv);
	DWORD ThreadIdSend;
	HANDLE hThreadSend = ::CreateThread(0, 0, SendThread, (LPVOID)&server, 0, &ThreadIdSend);
	server.Run();
	server.Release();
	CloseHandle(hThreadRecv);
	CloseHandle(hThreadSend);
}