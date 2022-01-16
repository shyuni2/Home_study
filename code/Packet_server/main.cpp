#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#include <string>
#include "Packet.h"
#pragma comment	(lib, "ws2_32.lib")

struct User
{
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	string m_Name;
	short       m_Port;
	void set(SOCKET sock, SOCKADDR_IN addr)
	{
		m_Sock = sock;
		m_Addr = addr;
		m_Name = inet_ntoa(addr.sin_addr);
		m_Port = ntohs(addr.sin_port);
	}
};

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	// 1. ��Ŷ ����
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2. ��Ŷ ����
	char* pMsg = (char*)&packet;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pMsg[SendSize],
			packet.ph.len - SendSize, 0);
		if (SendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		SendSize += SendByte;
	} while (SendSize < packet.ph.len);
	return SendSize;
}
int SendMsg(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pMsg[SendSize],
			packet.ph.len - SendSize, 0);
		if (SendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		SendSize += SendByte;
	} while (SendSize < packet.ph.len);
	return SendSize;
}
void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)  return;
	ret = listen(ListenSock, SOMAXCONN);
	if (ret == SOCKET_ERROR)  return;

	SOCKADDR_IN ClientAddr;
	int CALength = sizeof(ClientAddr);

	cout << " - ���� ������ - " << endl;

	u_long on = 1;
	ioctlsocket(ListenSock, FIONBIO, &on);

	// �迭�� ����ϸ� �������� ��Ȳ�� ����ؾ��ϱ� ������ list ���
	list<User> userlist;

	// ������ �� �۾��� �ݺ��Ǿ�� �������ڸ��� ������ ��츦 ���ټ� ����
	while (1)
	{
		SOCKET ClientSock = accept(ListenSock, (sockaddr*)&ClientAddr, &CALength);
		if (ClientSock == SOCKET_ERROR)
		{
			int Error = WSAGetLastError();
			if (Error != WSAEWOULDBLOCK)
			{
				cout << "ErrorCode=" << Error << endl;
				break;
			}
		}
		else
		{
			User user;
			user.set(ClientSock, ClientAddr);
			userlist.push_back(user);

			cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << ", port =" << ntohs(ClientAddr.sin_port) << " -> ����" << endl;
			u_long on = 1;
			ioctlsocket(ClientSock, FIONBIO, &on);
			cout << " - " << userlist.size() << " �� ������ - " << endl;
		}

		if (userlist.size() > 0)
		{
			list<User>::iterator iter;
			for (iter = userlist.begin(); iter != userlist.end(); )
			{
				User user = *iter;
				char szRecvBuffer[256] = { 0, };
				//��Ŷ��� �ޱ�
				UPACKET recvPacket;
				ZeroMemory(&recvPacket, sizeof(recvPacket));
				int RecvSize = 0;
				do {
					int RecvByte = recv(user.m_Sock, szRecvBuffer, PACKET_HEADER_SIZE, 0);
					RecvSize += RecvByte;
					if (RecvByte == 0)
					{
						closesocket(user.m_Sock);
						iter = userlist.erase(iter);
						cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << ", port =" << ntohs(ClientAddr.sin_port) << " -> ��������" << endl;
						continue; // ���� ������ ������ ������ �ȵ�
					}
					if (RecvByte == SOCKET_ERROR)
					{
						int Error = WSAGetLastError();
						if (Error != WSAEWOULDBLOCK)
						{
							iter = userlist.erase(iter);
							cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << ", port =" << ntohs(ClientAddr.sin_port) << " -> ������ ��������" << endl;
							break;
						}
						else
						{
							break;
						}
					}
				} while (RecvSize < PACKET_HEADER_SIZE);

				if (RecvSize == SOCKET_ERROR)
				{
					if (iter != userlist.end())
					{
						iter++;
					}
					continue;
				}

				memcpy(&recvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);
				// ������ �ޱ�
				RecvSize = 0;
				do {
					int RecvByte = recv(user.m_Sock, recvPacket.msg,
						recvPacket.ph.len - PACKET_HEADER_SIZE - RecvSize, 0);
					RecvSize += RecvByte;
					if (RecvByte == 0)
					{
						closesocket(user.m_Sock);
						iter = userlist.erase(iter);
						cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << ", port =" << ntohs(ClientAddr.sin_port) << " -> ��������" << endl;
						continue;
					}
					if (RecvByte == SOCKET_ERROR)
					{
						int Error = WSAGetLastError();
						if (Error != WSAEWOULDBLOCK)
						{
							iter = userlist.erase(iter);
							cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << ", port =" << ntohs(ClientAddr.sin_port) << " -> ������ ��������" << endl;
						}
						else
						{
							iter++;
						}
					}
				} while (RecvSize < recvPacket.ph.len - PACKET_HEADER_SIZE);

				Packet data;
				data.m_uPacket = recvPacket;
				ChatMsg recvdata;
				ZeroMemory(&recvdata, sizeof(recvdata));
				data >> recvdata.index >> recvdata.name >> recvdata.age >> recvdata.message;

				cout << "\n" << "[" << recvdata.name << "]" << recvdata.message;

				// ��Ŷ �ϼ�		
				list<User>::iterator Senditer;
				for (Senditer = userlist.begin();
					Senditer != userlist.end(); )
				{
					User user = *Senditer;
					int SendMsgSize = SendMsg(user.m_Sock, recvPacket);
					if (SendMsgSize < 0)
					{
						closesocket(user.m_Sock);
						Senditer = userlist.erase(Senditer);
						cout << "ip =" << inet_ntoa(ClientAddr.sin_addr) << ", port =" << ntohs(ClientAddr.sin_port) << " -> ������ ��������" << endl;
					}
					else
					{
						Senditer++;
					}
				}
				if (iter != userlist.end())
				{
					iter++;
				}
			}
		}
	}
	closesocket(ListenSock);
	WSACleanup();
}