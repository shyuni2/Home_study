#pragma once
#include "Protocol.h"
#include <string>

using namespace std;

// Packet data;
// data << header << index << name << damage << mesage;
// data >> header >> index >> name >> damage >> mesage;
class Packet
{
public:
	char*	m_pOffset;
	char* c_str() { return m_pOffset; }
	UPACKET m_uPacket;
	void PutData(const char* pData, int Size);
	void GetData(const char* pData, int Size);
public:
	Packet& operator << (int data);
	Packet& operator << (long data);
	Packet& operator << (short data);
	Packet& operator << (float data);
	Packet& operator << (byte data);
	Packet& operator << (char* data);
	Packet& operator << (DWORD data);
	Packet& operator << (string data);
public:
	Packet& operator >> (int& data);
	Packet& operator >> (long& data);
	Packet& operator >> (short& data);
	Packet& operator >> (float& data);
	Packet& operator >> (byte& data);
	Packet& operator >> (char* data);
	Packet& operator >> (DWORD& data);
	Packet& operator >> (string& data);
public:
	Packet();
	Packet(WORD iType);
	Packet(const Packet& packet);
	~Packet();
};

