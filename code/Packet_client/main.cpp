#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <list>
#include <conio.h>
#include "Packet.h"
#pragma comment (lib, "ws2_32.lib")

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	// 1. 패킷 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2. 패킷 전송
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
	// 패킷 생성
	Packet packet(type);
	packet << 1 << "이승현" << (short)26 << msg;
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
	} while (SendSize < packet.m_uPacket.ph.len); // 보낼 메세지 길이가 패킷헤더 길이보다 작을때까지 반복
	return SendSize;
}
int RecvPacketData(SOCKET sock, UPACKET& RecvPacket)
{
	// 데이터 받기
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, RecvPacket.msg, RecvPacket.ph.len - PACKET_HEADER_SIZE - RecvSize, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << "접속종료" << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "비정상 접속종료" << endl;
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
	// 패킷헤더 받기
	ZeroMemory(&RecvPacket, sizeof(RecvPacket));
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, szRecvBuffer, PACKET_HEADER_SIZE, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << "접속종료" << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "비정상 접속종료" << endl;
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

// 패킷을 추가해서 조금만 달라질뿐 기존과 변하는건 없다
void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa; // 함수인자로 사용할때는 항상 주소값을 전달하며 반드시 포인터형으로 반환해서 사용
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = inet_addr("192.168.0.87");
	int ret = connect(sock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		return;
	}
	cout << "접속 성공 >" << endl;

	// 논블로킹
	u_long on = 1;
	// ioctlsocket(논블로킹으로 만들 소켓, 소켓에대한 수행할 동작을 의미하는 커맨드, 0이면 블로킹모드가 되고 0이아닌값을 넣으면 논블로킹모드로 설정)
	ioctlsocket(sock, FIONBIO, &on);

	char szBuffer[256] = { 0, };
	int end = 0;

	while (1)
	{
		if (_kbhit() == 1) // 키보드 입력받는 함수
		{
			int Value = _getche();
			if (Value == '\r' && strlen(szBuffer) == 0)
			{
				cout << "정상 종료" << endl;
				break;
			}
			if (Value == '\r')
			{
				int SendByte = SendPacketData(sock, szBuffer, PACKET_CHAT_MSG);
				if (SendByte < 0)
				{
					cout << "비정상 종료" << endl;
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
				// 메세지 처리
				Packet data;
				data.m_uPacket = packet;
				ChatMsg RecvData;
				ZeroMemory(&RecvData, sizeof(RecvData));
				data >> RecvData.index >> RecvData.name >> RecvData.age >> RecvData.message;
				cout << "\n" << "[" << RecvData.name << "]" << RecvData.message;
			}
		}
	}
	cout << "접속종료" << endl;
	closesocket(sock);
	WSACleanup();
}