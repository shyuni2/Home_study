#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetWork.h"

class NetUser
{
public:
	bool m_Connect = false;
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_csName;
	short m_Port;
public:
	char m_RecvBuffer[2048];
	int  m_PacketPos; // 패킷의 시작주소
	int  m_WritePos;  // 현재의 저장 위치
	int  m_ReadPos;
public:
	HANDLE h_inEvent;
	HANDLE h_outEvent;
	HANDLE h_connectEvent;
	CRITICAL_SECTION m_cs;
public:
	list<NetUser> m_Userlist;
	list<Packet> m_packetPool;
public: 
	int SendData(SOCKET sock, UPACKET& packet);
	int SendData(SOCKET sock, char* msg, WORD type);
	int RecvPacketHeader(SOCKET sock, UPACKET& recvPacket);
	int RecvData(NetUser& user);
	int Broadcast(NetUser& user);
	int DispatchRead(char* RecvBuffer, int RecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

