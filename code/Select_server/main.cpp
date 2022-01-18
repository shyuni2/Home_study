#include "Network.h"

list<NetUser> userlist;

int SendMsg(SOCKET sock, char*msg, WORD type)
{
	// 1. ��Ŷ ����
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg)+PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2. ��Ŷ ����
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
int SendMsg(SOCKET sock, UPACKET& packet)
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
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	SOCKET clientSock = accept(sock,
		(sockaddr*)&clientAddr, &iLen);
	if (clientSock == SOCKET_ERROR)
	{		
		return -1;
	}
	else
	{
		NetUser user;
		user.set(clientSock, clientAddr);		
		userlist.push_back(user);
		cout << "ip =" << inet_ntoa(clientAddr.sin_addr) << " port =" << ntohs(clientAddr.sin_port) << "����" << endl;
		cout << userlist.size() << " �� ������ -" << endl;
	}
	return 1;
}
int RecvUser(NetUser& user)
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
	net.InitServer(SOCK_STREAM, 10000);	// TCP, ��Ʈ��ȣ

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	cout<< "- ���� ������ -" << endl;

	FD_SET rSet; // ���� ���԰ų� �޼��� �����߰ų�
	FD_SET wSet; // ���� �޼����� ���°ų�
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1;
	while (1)
	{
		// ����Ʈ���� ������ ������ �Ⱦ��� ���ŷ �ͺ��ŷ �ȵ���, ��� ������ ������ �Ź� �ݺ��Ҷ����� �ʱ�ȭ�ϰ� ������ �������
		
		// �ʱ�ȭ �۾�, FD_SET�� ����ü �迭 �����Ⱚ�� ������������ FD_ZERO�� �̿��� �ʱ�ȭ�� ����ؾ���
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		// ����
		FD_SET(net.m_ListenSock, &rSet); // �������Ͽ� ���� ���԰ų� �޼����� ���´ٴ� �� Ȯ����
		list<NetUser>::iterator userIter;
		for (userIter = userlist.begin();
			 userIter != userlist.end();)
		{
			if ((*userIter).m_bConnect == false)
			{
				cout << (*userIter).m_csName << " ��������" << endl;
				userIter = userlist.erase(userIter);	
				continue;
			}
			FD_SET((*userIter).m_Sock, &rSet);
			// ���� user���� ���� ��Ŷ�� ������
			if ((*userIter).m_PacketPool.size() > 0)
			{
				FD_SET((*userIter).m_Sock, &wSet);
			}
			userIter++;
		}
		// ���� ���ŷ ���±⶧���� ��ȯ���� ���� �۾�����
		int iRet = select(0, &rSet, &wSet, NULL, &timeout);
		if (iRet == 0)
		{
			continue;
		}
		if (FD_ISSET(net.m_ListenSock, &rSet)) // ��ȯ�Ǹ� ���� ���԰ų� �޼����� ���´ٴ°���
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
				int iRet= RecvUser(user);
				if (iRet <= 0)
				{
					user.m_bConnect = false;					
				}
			}
		}
		for (NetUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &wSet)) // ��ȯ�Ǹ� ���� ���԰ų� �޼����� ���´ٴ°���
			{
				if (user.m_PacketPool.size() > 0)
				{
					list<Packet>::iterator iter;
					for (iter = user.m_PacketPool.begin(); iter != user.m_PacketPool.end(); )
					{
						for (NetUser& senduser : userlist)
						{							
							int iRet = SendMsg(senduser.m_Sock, (*iter).m_uPacket);							
							if (iRet <= 0) // ���ú� ���� 0���� �۰ų� ������ ������ ���� �ȵƴٴ� ��
							{
								senduser.m_bConnect = false; // ���� ����
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