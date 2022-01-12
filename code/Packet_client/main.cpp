#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <list>
#include "Packet.h"
#pragma comment (lib, "ws2_32.lib")

int SendPacketData(SOCKET sock, char* msg, WORD type)
{
	// ��Ŷ ����
	Packet packet(type);
	packet << 1 << "�̽���" << (short)26 << msg;
	Packet PacketTest(packet);
	ChatMsg RecvData;
	ZeroMemory(&RecvData, sizeof(RecvData));
	PacketTest >> RecvData.index >> RecvData.name >> RecvData.age >> RecvData.message;
	char* pData = (char*)&packet.m_uPacket;
	int SendSize = 0;
	do { 
		int SendByte = send(sock, &pData[SendSize], packet.m_uPacket.ph.len, 0);
		if (SendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		SendSize += SendByte;
	} while (SendSize < packet.m_uPacket.ph.len); // ���� �޼��� ���̰� ��Ŷ��� ���̺��� ���������� �ݺ�
	return SendSize;
}

int RecvPacketHeader(SOCKET sock, UPACKET& RecvPacket) 
{


}

// ��Ŷ�� �߰��ؼ� ���ݸ� �޶����� ������ ���ϴ°� ����
void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa; // �Լ����ڷ� ����Ҷ��� �׻� �ּҰ��� �����ϸ� �ݵ�� ������������ ��ȯ�ؼ� ���
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	// ���ε�
	// bind(Ŭ���̾�Ʈ ������ ������ �������� ���� ����, �����ּ� ����ü, �����ּ� ����ü ����
	int ret = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		return;
	}
	// ���� 
	// listen(bind�Լ��� ���� �������ּҿ� ������Ʈ��ȣ�� ������ ����, ���Ӱ����� Ŭ���̾�Ʈ ����)
	ret = listen(ListenSock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		return;
	}

	SOCKADDR_IN ClientAddr;
	int CALength = sizeof(ClientAddr);

	cout << "���� ������ >" << endl;

	// ����ŷ
	u_long on = 1;
	// ioctlsocket(����ŷ���� ���� ����, ���Ͽ����� ������ ������ �ǹ��ϴ� Ŀ�ǵ�, 0�̸� ���ŷ��尡 �ǰ� 0�̾ƴѰ��� ������ ����ŷ���� ����)
	ioctlsocket(ListenSock, FIONBIO, &on);

	// �迭�� ����ϸ� �������� ��Ȳ�� ����ؾ��ϱ⶧���� list ���
	list<User>userlist;

	//������ ���۾��� �ݺ��Ǿ�� �������ڸ��� ������ ��츦 ���ټ�����
	while (1)
	{
		SOCKET ClientSock = accept(ListenSock, (sockaddr*)&ClientAddr, &CALength);
		// Ŭ�� ����
		if (ClientSock == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
				cout << "ErrorCode = " << error << endl;
				break;
			}
		}
		else
		{
			User user;
			user.set(ClientSock, ClientAddr);
			userlist.push_back(user);
			cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> ������" << endl;

			u_long on = 1;
			ioctlsocket(ClientSock, FIONBIO, &on);

			cout << userlist.size() << "�� ���� �� �Դϴ�." << endl;
		}

		// ��������Ʈ ����, ���⼭ ��Ŷ�߰�
		if (userlist.size() > 0)
		{
			list<User>::iterator useriter;
			for (useriter = userlist.begin(); useriter != userlist.end();)
			{
				User user = *useriter;
				char szRecvBuffer[256] = { 0, };
				// ��Ŷ��� �ޱ�
				UPACKET RecvPacket;
				ZeroMemory(&RecvPacket, sizeof(RecvPacket));
				int RecvSize = 0;
				do {
					int RecvByte = recv(user.u_Sock, szRecvBuffer, PACKET_HEADER_SIZE, 0);
					RecvSize += RecvByte;
					if (RecvByte == 0) // ���ú�Ȱ� 0�̸� Ŭ�� ����
					{
						closesocket(user.u_Sock);
						useriter = userlist.erase(useriter);
						cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> ��������" << endl;
						continue; // ������ ������ �ȵ�
					}
					if (RecvByte == SOCKET_ERROR)
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							useriter = userlist.erase(useriter);
							cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> ������ ��������" << endl;
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
					if (useriter != userlist.end())
					{
						useriter++;
					}
					continue;
				}
				memcpy(&RecvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);

				// ������ �ޱ�
				RecvSize = 0;
				do {
					int RecvByte = recv(user.u_Sock, RecvPacket.msg, RecvPacket.ph.len - PACKET_HEADER_SIZE - RecvSize, 0);
					RecvSize += RecvByte;
					if (RecvByte == 0)
					{
						closesocket(user.u_Sock);
						useriter = userlist.erase(useriter);
						cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> ��������" << endl;
						continue;
					}
					if (RecvByte == SOCKET_ERROR)
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							useriter = userlist.erase(useriter);
							cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> ������ ��������" << endl;
						}
						else
						{
							useriter++;
						}
					}
				} while (RecvSize < RecvPacket.ph.len - PACKET_HEADER_SIZE); // Ȥ�ø𸣴� �߸� �����Ͱ� �������� �ֱ� ����

				Packet data;
				data.m_uPacket = RecvPacket;
				ChatMsg RecvData;
				ZeroMemory(&RecvData, sizeof(RecvData));
				data >> RecvData.index >> RecvData.name >> RecvData.age >> RecvData.message;
				cout << "[" << RecvData.name << "]" << RecvData.message << endl;

				// ��Ŷ�ϼ�
				list<User>::iterator Senditer;
				for (Senditer = userlist.begin(); Senditer != userlist.end();)
				{
					User user = *Senditer;
					int SendMsgSize = SendMsg(user.u_Sock, RecvPacket);
					if (SendMsgSize < 0)
					{
						closesocket(user.u_Sock);
						Senditer = userlist.erase(Senditer);
						cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> ������ ��������" << endl;
					}
					else
					{
						Senditer++;
					}
				}
				if (Senditer != userlist.end())
				{
					Senditer++;
				}
			}
		}
	}
	closesocket(ListenSock);
	WSACleanup();
}