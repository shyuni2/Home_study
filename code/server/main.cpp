
#include "../lib/NetUser.h"

NetUser user;
list<NetUser> m_Userlist;
CRITICAL_SECTION m_cs;

DWORD WINAPI RecvThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		EnterCriticalSection(&m_cs);
		list<NetUser>::iterator userIter;
		for (userIter = m_Userlist.begin(); userIter != m_Userlist.end();)
		{
			int ret = user.RecvData(*userIter);
			if (ret <= 0)
			{
				userIter = m_Userlist.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		LeaveCriticalSection(&m_cs);
		Sleep(1);
	}
}
DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		EnterCriticalSection(&m_cs);

		list<NetUser>::iterator userIter;
		for (userIter =m_Userlist.begin(); userIter != m_Userlist.end();)
		{
			int ret = user.Broadcast(*userIter);
			if (ret <= 0)
			{
				userIter = m_Userlist.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		LeaveCriticalSection(&m_cs);

		Sleep(1);
	}
}



void main()
{
	NetWork net;
	net.InitServer(1);
	DWORD ThreadIdRecv;
	HANDLE hThreadRecv = ::CreateThread(0, 0, RecvThread, (LPVOID)&net, 0, &ThreadIdRecv);
	DWORD ThreadIdSend;
	HANDLE hThreadSend = ::CreateThread(0, 0, SendThread, (LPVOID)&net, 0, &ThreadIdSend);
	net.Run();
	net.Release();
	CloseHandle(hThreadRecv);
	CloseHandle(hThreadSend);
}