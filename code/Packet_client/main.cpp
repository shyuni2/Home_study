#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include "Packet.h"
#pragma comment	(lib, "ws2_32.lib")

// ��� 1
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
// ��� 2
int SendPacketData(SOCKET sock, char* msg, WORD type)
{
	// 1. ��Ŷ ����
	Packet tPacket(type);
	tPacket << 999 << "�̽���" << (short)26 << msg;
	Packet tPacketTest(tPacket);
	ChatMsg recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	tPacketTest >> recvdata.index >> recvdata.name >> recvdata.age >> recvdata.message;
	char* pData = (char*)&tPacket.m_uPacket;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pData[SendSize], tPacket.m_uPacket.ph.len - SendSize, 0);
		if (SendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		SendSize += SendByte;
	} while (SendSize < tPacket.m_uPacket.ph.len); // ���� �޼��� ���̰� ��Ŷ��� ���̺��� ���������� �ݺ�
	return SendSize;
}
int RecvPacketHeader(SOCKET sock, UPACKET& recvPacket)
{
	char szRecvBuffer[256] = { 0, };
	//��Ŷ��� �ޱ�
	ZeroMemory(&recvPacket, sizeof(recvPacket));
	bool bRun = true;
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, szRecvBuffer, PACKET_HEADER_SIZE, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << " ��������" << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int Error = WSAGetLastError();
			if (Error != WSAEWOULDBLOCK)
			{
				cout << " ������ ��������" << endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (RecvSize < PACKET_HEADER_SIZE);
	memcpy(&recvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);
	return 1;
}
int RecvPacketData(SOCKET sock, UPACKET& recvPacket)
{
	// ������ �ޱ�
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, recvPacket.msg, recvPacket.ph.len - PACKET_HEADER_SIZE - RecvSize, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << " ��������" << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int Error = WSAGetLastError();
			if (Error != WSAEWOULDBLOCK)
			{
				cout << " ������ ��������" << endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (RecvSize < recvPacket.ph.len - PACKET_HEADER_SIZE);
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
	SOCKADDR_IN sa;  // �Լ����ڷ� ����Ҷ��� �׻� �ּҰ��� �����ϸ� �ݵ�� ������������ ��ȯ�ؼ� ���
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1);
	sa.sin_addr.s_addr = inet_addr("192.168.219.105");
	int ret = connect(sock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		return;
	}
	cout << "���Ӽ���!" << endl;

	// ����ŷ
	u_long on = 1;
	// ioctlsocket(����ŷ���� ���� ����, ���Ͽ����� ������ ������ �ǹ��ϴ� Ŀ�ǵ�, 0�̸� ���ŷ��尡 �ǰ� 0�̾ƴѰ��� ������ ����ŷ���� ����)
	ioctlsocket(sock, FIONBIO, &on);

	char szBuffer[256] = { 0, };
	int End = 0;
	while (1)
	{
		if (_kbhit() == 1) // Ű���� �Է¹޴� �Լ�
		{
			int Value = _getche();
			if (Value == '\r' && strlen(szBuffer) == 0)
			{
				cout << "���� �����.." << endl;
				break;
			}
			if (Value == '\r')
			{
				// ��� 1
				//int SendByte = SendMsg(sock, szBuffer, PACKET_CHAT_MSG);
				// ��� 2
				int SendByte = SendPacketData(sock, szBuffer, PACKET_CHAT_MSG);
				if (SendByte < 0)
				{
					cout << "������ ����" << endl;
					break;
				}
				End = 0;
				ZeroMemory(szBuffer, sizeof(char) * 256);
			}
			else
			{
				szBuffer[End++] = Value;
			}
		}
		else
		{
			UPACKET packet;
			int ret = RecvPacketHeader(sock, packet);
			if (ret < 0) break;
			if (ret == 1)
			{
				int ret = RecvPacketData(sock, packet);
				if (ret < 0) break;
				if (ret == 0) continue;
				// �޼��� ó��
				Packet data;
				data.m_uPacket = packet;
				ChatMsg recvdata;
				ZeroMemory(&recvdata, sizeof(recvdata));
				data >> recvdata.index >> recvdata.name >> recvdata.age >> recvdata.message;
				cout << "\n" << "[" << recvdata.name << "]" << recvdata.message;
			}
		}
	}
	cout << "��������" << endl;
	closesocket(sock);
	WSACleanup();
}