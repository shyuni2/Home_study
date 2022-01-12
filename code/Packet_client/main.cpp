#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <list>
#include <conio.h>
#include "Packet.h"
#pragma comment (lib, "ws2_32.lib")

struct User
{
	SOCKET u_Sock;
	SOCKADDR_IN u_Addr;
	string u_Name;
	short u_Port;
	void set(SOCKET USock, SOCKADDR_IN UAddr)
	{
		u_Sock = USock;
		u_Addr = UAddr;
		u_Name = inet_ntoa(UAddr.sin_addr);
		u_Port = ntohs(UAddr.sin_port);
	}
};

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
int RecvPacketData(SOCKET sock, UPACKET& RecvPacket)
{
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, RecvPacket.msg, RecvPacket.ph.len - PACKET_HEADER_SIZE - RecvSize, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << "��������" << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "������ ��������" << endl;
			}
			else
			{
				return 0;
			}
		}
	} while (RecvSize < PACKET_HEADER_SIZE);
	return 1;
}

int RecvPacketHeader(SOCKET sock, UPACKET& RecvPacket) 
{
	char szRecvBuffer[256] = { 0, };
	// ��Ŷ��� �ޱ�
	ZeroMemory(&RecvPacket, sizeof(RecvPacket));
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, szRecvBuffer, PACKET_HEADER_SIZE, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << "��������" << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "������ ��������" << endl;
			}
			else
			{
				return 0;
			}
		}
	} while (RecvSize < PACKET_HEADER_SIZE);
	memcpy(&RecvPacket.ph.len, szRecvBuffer, PACKET_HEADER_SIZE);
	return 1;
}

// ��Ŷ�� �߰��ؼ� ���ݸ� �޶����� ������ ���ϴ°� ����
void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa; // �Լ����ڷ� ����Ҷ��� �׻� �ּҰ��� �����ϸ� �ݵ�� ������������ ��ȯ�ؼ� ���
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = inet_addr("192.168.0.117");
	int ret = connect(sock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		return;
	}
	cout << "���� ���� >" << endl;

	// ����ŷ
	u_long on = 1;
	// ioctlsocket(����ŷ���� ���� ����, ���Ͽ����� ������ ������ �ǹ��ϴ� Ŀ�ǵ�, 0�̸� ���ŷ��尡 �ǰ� 0�̾ƴѰ��� ������ ����ŷ���� ����)
	ioctlsocket(sock, FIONBIO, &on);

	char szBuffer[256] = { 0, };
	int end = 0;

	while (1)
	{
		if (_kbhit() == 1) // Ű���� �Է¹޴� �Լ�
		{
			int Value = _getche();
			if (Value == '\r' && strlen(szBuffer) == 0)
			{
				cout << "���� ����" << endl;
				break;
			}
			if (Value == '\r')
			{
				int SendByte = SendPacketData(sock, szBuffer, PACKET_CHAT_MSG);
				if (SendByte < 0)
				{
					cout << "������ ����" << endl;
					break;
				}
				end = 0;
				ZeroMemory(szBuffer, sizeof(char) * 256);
			}
			else
			{
				szBuffer[end++] = Value;
			}
		}
		else
		{
			UPACKET packet;
			int ret = RecvPacketHeader(sock, packet);
			if (ret < 0)
			{
				break;
			}
			if (ret == 0)
			{
				continue;
			}
			if (ret == 1)
			{
				// �޼��� ó��
				Packet data;
				data.m_uPacket = packet;
				ChatMsg RecvData;
				ZeroMemory(&RecvData, sizeof(RecvData));
				data >> RecvData.index >> RecvData.name >> RecvData.age >> RecvData.message;
				cout << "[" << RecvData.name << "]" << RecvData.message;
			}
		}
	}
	cout << "��������" << endl;
	closesocket(sock);
	WSACleanup();
}