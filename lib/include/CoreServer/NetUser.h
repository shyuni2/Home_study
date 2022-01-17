#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ServerObj.h"
#include "Packet.h"

struct TOV // 오버랩구조체
{
	OVERLAPPED ov; // 밑에 있으면 캐스팅 안됨
	int type;
};

class NetUser 
{
public:
	bool m_bConnect = false;
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	string m_Name;
	short m_iPort;
	// 버퍼기능 추가
	TOV m_ovRecv;
	TOV m_ovSend;
	WSABUF m_wsaRecvBuffer;
	WSABUF m_wsaSendBuffer;
	char m_szRecv[256];
	char m_szSend[256];
public:
	char m_szRecvBuffer[2048];
	int m_iPacketPos; // 패킷의 시작주소
	int m_iWritePos;  // 현재의 저장 위치
	int m_iReadPos;
	list<Packet> m_PacketPool;
public:
	int Dispatch(DWORD dwTrans, TOV* tov);
	int DispatchRecv(char* szRecvBuffer, int iRecvByte);
	int DispatchSend(DWORD dwTrans);
	void set(SOCKET sock, SOCKADDR_IN addr);
	int Recv();
	bool DisConnect();
public:
	NetUser();
	virtual ~NetUser();
};

