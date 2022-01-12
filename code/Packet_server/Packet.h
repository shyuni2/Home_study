#pragma once
#include "Protocol.h"
#include <string>

using namespace std;

// 패킷 딩터 진행방식
// data << packet << index << name << age << message ;
// data >> packet >> index >> name >> age >> message ; 순서대로 처리해줘야함, 이런 구조로 데이터를 주고받겠다는 뜻

class Packet
{
public:
	char* m_Offset;
	UPACKET m_uPacket;
public:
	void PutData(const char* pData, int Size); // 데이터 주고
	void GetData(const char* pData, int Size); // 데이터 받기
public: // 데이터형이 여러가지일수 있으니 데이터형에 따라서 연산자 오버로딩을 해준다
	Packet& operator << (int data);
	Packet& operator << (long data);
	Packet& operator << (short data);
	Packet& operator << (float data);
	Packet& operator << (byte data);
	Packet& operator << (char* data);
	Packet& operator << (DWORD data);
	Packet& operator << (string data);
public:
	Packet& operator >> (int data);
	Packet& operator >> (long data);
	Packet& operator >> (short data);
	Packet& operator >> (float data);
	Packet& operator >> (byte data);
	Packet& operator >> (char* data);
	Packet& operator >> (DWORD data);
	Packet& operator >> (string data);
public:
	Packet();
	Packet(WORD Type);
	Packet(const Packet& packet);
	~Packet();
};

