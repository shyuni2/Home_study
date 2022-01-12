#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <list>
#include "Packet.h"
#pragma comment (lib, "ws2_32.lib")

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

int RecvPacketHeader(SOCKET sock, UPACKET& RecvPacket) 
{


}

// 패킷을 추가해서 조금만 달라질뿐 기존과 변하는건 없다
void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa; // 함수인자로 사용할때는 항상 주소값을 전달하며 반드시 포인터형으로 반환해서 사용
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	// 바인드
	// bind(클라이언트 접속을 수용할 목적으로 만든 소켓, 소켓주소 구조체, 소켓주소 구조체 길이
	int ret = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		return;
	}
	// 리슨 
	// listen(bind함수에 의해 아이피주소와 지역포트번호가 설정된 소켓, 접속가능한 클라이언트 개수)
	ret = listen(ListenSock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		return;
	}

	SOCKADDR_IN ClientAddr;
	int CALength = sizeof(ClientAddr);

	cout << "서버 실행중 >" << endl;

	// 논블로킹
	u_long on = 1;
	// ioctlsocket(논블로킹으로 만들 소켓, 소켓에대한 수행할 동작을 의미하는 커맨드, 0이면 블로킹모드가 되고 0이아닌값을 넣으면 논블로킹모드로 설정)
	ioctlsocket(ListenSock, FIONBIO, &on);

	// 배열을 사용하면 빠졌을떄 상황을 고려해야하기때문에 list 사용
	list<User>userlist;

	//서버는 이작업이 반복되어야 실행하자마자 꺼지는 경우를 없앨수있음
	while (1)
	{
		SOCKET ClientSock = accept(ListenSock, (sockaddr*)&ClientAddr, &CALength);
		// 클라 소켓
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
			cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> 접속중" << endl;

			u_long on = 1;
			ioctlsocket(ClientSock, FIONBIO, &on);

			cout << userlist.size() << "명 접속 중 입니다." << endl;
		}

		// 유저리스트 관리, 여기서 패킷추가
		if (userlist.size() > 0)
		{
			list<User>::iterator useriter;
			for (useriter = userlist.begin(); useriter != userlist.end();)
			{
				User user = *useriter;
				char szRecvBuffer[256] = { 0, };
				// 패킷헤더 받기
				UPACKET RecvPacket;
				ZeroMemory(&RecvPacket, sizeof(RecvPacket));
				int RecvSize = 0;
				do {
					int RecvByte = recv(user.u_Sock, szRecvBuffer, PACKET_HEADER_SIZE, 0);
					RecvSize += RecvByte;
					if (RecvByte == 0) // 리시브된게 0이면 클라 나감
					{
						closesocket(user.u_Sock);
						useriter = userlist.erase(useriter);
						cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> 접속종료" << endl;
						continue; // 서버는 꺼지면 안됨
					}
					if (RecvByte == SOCKET_ERROR)
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							useriter = userlist.erase(useriter);
							cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> 비정상 접속종료" << endl;
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

				// 데이터 받기
				RecvSize = 0;
				do {
					int RecvByte = recv(user.u_Sock, RecvPacket.msg, RecvPacket.ph.len - PACKET_HEADER_SIZE - RecvSize, 0);
					RecvSize += RecvByte;
					if (RecvByte == 0)
					{
						closesocket(user.u_Sock);
						useriter = userlist.erase(useriter);
						cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> 접속종료" << endl;
						continue;
					}
					if (RecvByte == SOCKET_ERROR)
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							useriter = userlist.erase(useriter);
							cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> 비정상 접속종료" << endl;
						}
						else
						{
							useriter++;
						}
					}
				} while (RecvSize < RecvPacket.ph.len - PACKET_HEADER_SIZE); // 혹시모르는 잘린 데이터가 있을수도 있기 때문

				Packet data;
				data.m_uPacket = RecvPacket;
				ChatMsg RecvData;
				ZeroMemory(&RecvData, sizeof(RecvData));
				data >> RecvData.index >> RecvData.name >> RecvData.age >> RecvData.message;
				cout << "[" << RecvData.name << "]" << RecvData.message << endl;

				// 패킷완성
				list<User>::iterator Senditer;
				for (Senditer = userlist.begin(); Senditer != userlist.end();)
				{
					User user = *Senditer;
					int SendMsgSize = SendMsg(user.u_Sock, RecvPacket);
					if (SendMsgSize < 0)
					{
						closesocket(user.u_Sock);
						Senditer = userlist.erase(Senditer);
						cout << "IP = " << inet_ntoa(ClientAddr.sin_addr) << "PORT = " << ntohs(ClientAddr.sin_port) << " ==> 비정상 접속종료" << endl;
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