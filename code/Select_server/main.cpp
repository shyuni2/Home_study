#include "Network.h"

list<NetUser> userlist;

int SendData(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize], packet.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.ph.len);
	return iSendSize;
}
int AddUser(SOCKET sock)
{
	SOCKADDR_IN ClientAddr;
	int iLen = sizeof(ClientAddr);
	SOCKET ClientSock = accept(sock,
		(sockaddr*)&ClientAddr, &iLen);
	if (ClientSock == SOCKET_ERROR)
	{		
		return -1;
	}
	else
	{
		NetUser user;
		user.set(ClientSock, ClientAddr);		
		userlist.push_back(user);
		cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << " port =" << ntohs(ClientAddr.sin_port) << "접속" << endl;
		cout << userlist.size() << " 명 접속중 -" << endl;
	}
	return 1;
}
int RecvData(NetUser& user)
{
	char szRecvBuffer[1024] = { 0, };	
	int iRecvByte = recv(user.m_Sock, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{		
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		return -1;
	}	
	user.DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}

void main()
{
	Network net;
	net.IniNetwork();
	net.InitServer(SOCK_STREAM, 10000);	// TCP, 포트번호

	SOCKADDR_IN ClientAddr;
	int iLen = sizeof(ClientAddr);
	cout<< "- 서버 가동중 -" << endl;

	FD_SET rSet; // 누가 들어왔거나 메세지 도착했거나
	FD_SET wSet; // 누가 메세지를 보냈거나
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1;
	while (1)
	{
		// 셀렉트쓰면 좋은점 스레드 안쓰고 블로킹 넌블로킹 안따짐, 대신 사용법이 귀찮음 매번 반복할때마다 초기화하고 설정을 해줘야함
		
		// 초기화 작업, FD_SET은 구조체 배열 쓰레기값이 있을수있으니 FD_ZERO를 이용해 초기화를 사용해야함
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		// 설정
		FD_SET(net.m_ListenSock, &rSet); // 리슨소켓에 누가 들어왔거나 메세지를 보냈다는 걸 확인함
		list<NetUser>::iterator userIter;
		for (userIter = userlist.begin(); userIter != userlist.end();)
		{
			if ((*userIter).m_bConnect == false)
			{
				cout << (*userIter).m_csName << " 접속종료" << endl;
				userIter = userlist.erase(userIter);	
				continue;
			}
			FD_SET((*userIter).m_Sock, &rSet);
			// 만약 user에서 보낸 패킷이 있으면
			if ((*userIter).m_PacketPool.size() > 0)
			{
				FD_SET((*userIter).m_Sock, &wSet);
			}
			userIter++;
		}
		// 현재 블로킹 상태기때문에 반환값에 따라서 작업가능
		int iRet = select(0, &rSet, &wSet, NULL, &timeout);
		if (iRet == 0)
		{
			continue;
		}
		if (FD_ISSET(net.m_ListenSock, &rSet)) // 반환되면 누가 들어왔거나 메세지를 보냈다는거임
		{
			if (AddUser(net.m_ListenSock)<= 0)
			{
				break;
			}
		}
		for (NetUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &rSet))
			{
				int iRet= RecvData(user);
				if (iRet <= 0)
				{
					user.m_bConnect = false;					
				}
			}
		}
		for (NetUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &wSet)) // 반환되면 누가 들어왔거나 메세지를 보냈다는거임
			{
				if (user.m_PacketPool.size() > 0)
				{
					list<Packet>::iterator iter;
					for (iter = user.m_PacketPool.begin(); iter != user.m_PacketPool.end(); )
					{
						for (NetUser& senduser : userlist)
						{							
							int iRet = SendData(senduser.m_Sock, (*iter).m_uPacket);
							if (iRet <= 0) // 리시브 값이 0보다 작거나 같으면 유저랑 연결 안됐다는 뜻
							{
								senduser.m_bConnect = false; // 연결 실패
							}
						}
						iter = user.m_PacketPool.erase(iter);
					}
				}
			}
		}
	}	
	net.CloseNetwork();
}