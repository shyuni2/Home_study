#include "Packet.h"

// 함수
void Packet::PutData(const char* pData, int Size) // 데이터 주고
{
	m_uPacket.ph.len += Size; 
	memcpy(m_Offset, pData, Size);
	m_Offset += Size; // 패킷에 데이터를 넣을 때 주소가 포인터로 되어있기 때문에 시작지점 위치를 알기 위해서 길이만큼 추가해줌
};
void Packet::GetData(const char* pData, int Size)// 데이터 받기
{
	memcpy(const_cast<char*>(pData), m_Offset, Size); // const가 붙어있기때문에 const_cast 캐스팅
	m_Offset += Size;
};

// << 오버로딩
Packet& Packet::operator<< (int data)
{
	PutData((char*)&data, sizeof(int));
	return *this;
}
Packet& Packet::operator<< (long data)
{
	PutData((char*)&data, sizeof(long));
	return *this;
}
Packet& Packet::operator<< (short data)
{
	PutData((char*)&data, sizeof(short));
	return *this;
}
Packet& Packet::operator<< (float data)
{
	PutData((char*)&data, sizeof(float));
	return *this;
}
Packet& Packet::operator<< (char* data) // 쭉 데이터형을 달면되는데 이제 문자형에선 고려해야할 사항이 있다
{
	PutData(data, strlen(data) + 1); // 길이에서 +1하는이유 : 문자뒤엔 널값이 들어간다 한칸 비워줌
	return *this;
}
Packet& Packet::operator<< (string data)
{
	PutData(data.c_str(), data.size() + 1); // string형을 const char*형을 반환할때 사용하는 함수 c_str(), string형의 크기를 출력해주는 함수 size()
	return *this;
}

// >> 오버로딩
Packet& Packet::operator>> (int data)
{
	GetData((char*)&data, sizeof(int));
	return *this;
}
Packet& Packet::operator>> (long data)
{
	GetData((char*)&data, sizeof(long));
	return *this;
}
Packet& Packet::operator>> (short data)
{
	GetData((char*)&data, sizeof(short));
	return *this;
}
Packet& Packet::operator>> (float data)
{
	GetData((char*)&data, sizeof(float));
	return *this;
}
Packet& Packet::operator>> (char* data) // 쭉 데이터형을 달면되는데 이제 문자형에선 고려해야할 사항이 있다
{
	GetData(data, strlen(data) + 1); // 길이에서 +1하는이유 : 문자뒤엔 널값이 들어간다 한칸 비워줌
	return *this;
}
Packet& Packet::operator>> (string data)
{
	GetData(data.c_str(), data.size() + 1); // string형을 const char*형을 반환할때 사용하는 함수 c_str(), string형의 크기를 출력해주는 함수 size()
	return *this;
}

// 생성자
Packet::Packet()
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET)); // 메모리 초기화 
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_Offset = m_uPacket.msg; // m_Offset은 m_uPacket의 메세지를 담음
}
Packet::Packet(WORD Type)
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET)); // 메모리 초기화 
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_uPacket.ph.type = Type;
	m_Offset = m_uPacket.msg; // m_Offset은 m_uPacket의 메세지를 담음
}
Packet::Packet(const Packet& packet)
{
	m_uPacket.ph = packet.m_uPacket.ph;
	memcpy(m_uPacket.msg, packet.m_uPacket.msg, sizeof(char) * 4096);
	m_Offset = m_uPacket.msg; // m_Offset은 m_uPacket의 메세지를 담음
}
Packet::~Packet()
{

}