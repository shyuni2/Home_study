#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <list>
#include <conio.h>
#include "Packet.h"
#pragma comment (lib, "ws2_32.lib")

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
		int SendByte = send(sock, &pMsg[SendSize], packet.ph.len - SendSize, 0);
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
	// ������ �ޱ�
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
				return -1;
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
	sa.sin_addr.s_addr = inet_addr("192.168.0.87");
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
			if (ret == 1)
			{
				int ret = RecvPacketData(sock, packet);
				if (ret < 0)
				{
					break;
				}
				if (ret == 0)
				{
					continue;
				}
				// �޼��� ó��
				Packet data;
				data.m_uPacket = packet;
				ChatMsg RecvData;
				ZeroMemory(&RecvData, sizeof(RecvData));
				data >> RecvData.index >> RecvData.name >> RecvData.age >> RecvData.message;
				cout << "\n" << "[" << RecvData.name << "]" << RecvData.message;
			}
		}
	}
	cout << "��������" << endl;
	closesocket(sock);
	WSACleanup();
}