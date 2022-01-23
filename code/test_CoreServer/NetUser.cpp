#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetUser.h"

int NetUser::DispatchRead(char* RecvBuffer, int RecvByte)
{
	// 데이터가 짤려서 들어올수 있기 때문에 어디가 시작인지 모르니까 시작위치를 바꿔서 더 받아준다
	if (m_WritePos + RecvByte >= 2048)
	{
		if (m_ReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_PacketPos], m_ReadPos);
		}
		m_PacketPos = 0;
		m_WritePos = m_ReadPos; // 잘린 자투리가 부족할때 더 받음
	}
	memcpy(&m_szRecvBuffer[m_WritePos], RecvBuffer, RecvByte); // 데이터가 얼만큼 들어왔는지 확인할수있기 때문에 받은 양만큼 넣어줌
	m_WritePos += RecvByte;// 버퍼에 이전에 저장된 위치
	m_ReadPos += RecvByte; // 패킷시작 위치로부터 받은 바이트

	if (m_ReadPos >= PACKET_HEADER_SIZE)
	{
		// 패킷 해석 가능
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_PacketPos];
		// 적어도 1개의 패킷은 도착했다.
		if (pPacket->ph.len <= m_ReadPos)
		{
			do {
				Packet tPacket(pPacket->ph.type);
				memcpy(&tPacket.m_uPacket, &m_szRecvBuffer[m_PacketPos], pPacket->ph.len); // 하나의 패킷 완성
				m_PacketPool.push_back(tPacket); // 완성된 패킷 푸시

				// 잔여량있으면 다음패킷 처리
				m_PacketPos += pPacket->ph.len;
				m_ReadPos -= pPacket->ph.len;
				if (m_ReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szRecvBuffer[m_PacketPos];
			} while (pPacket->ph.len <= m_ReadPos);
		}
	}
	return 1;
}

void NetUser::set(SOCKET sock, SOCKADDR_IN addr)
{
	m_bConnect = true; // 연결 성공
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_PacketPos = 0;
	m_WritePos = 0;
	m_ReadPos = 0;
	int m_WritePos;  // 현재의 저장 위치
	int m_ReadPos;

	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}