#include "NetUser.h"

int NetUser::DispatchRead(char* szRecvBuffer, int iRecvByte)
{
	// 데이터가 짤려서 들어올수있기 때문에 어디가 시작인지 모르니까 시작위치를 바꿔서 더 받아줌
	if (m_WritePos + iRecvByte >= 2048)
	{
		if (m_ReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_PacketPos], m_ReadPos);
		}
		m_ReadPos = 0;
		m_WritePos = m_ReadPos; // 잘린 짜투리가 부족할때 더 받음
	}
	// 얼만큼 데이터가 들어왔는지 확인할수있기 때문에 받은 양 만큼 넣어주고
	memcpy(&m_szRecvBuffer[m_WritePos], szRecvBuffer, iRecvByte);
	m_WritePos += iRecvByte; // 버퍼의 이전에 저장된 위치
	m_ReadPos += iRecvByte; // 패킷 시작 위치로부터 받은 바이트

	if (m_ReadPos >= PACKET_HEADER_SIZE) 
	{
		// 패킷 해석가능
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_PacketPos]; // 모든데이터가 문자열에 있으니 포인터사용, 처음엔 0번주소가 패킷
		// 적어도 한개의 패킷은 도착했다
		if (pPacket->ph.len <= m_ReadPos)
		{
			do {
				Packet tPacket(pPacket->ph.type); // UPacket, Packet 상관없음
				memcpy(&tPacket, &m_szRecvBuffer[m_PacketPos], pPacket->ph.len); // 하나의 패킷 완성
				m_PacketPool.push_back(tPacket); // 완성된 패킷 푸시

				// 잔여량이 있으면 또 받을수있음
				// 다음패킷 처리
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
	m_Connect = true; // 연결 성공
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_PacketPos = 0;
	m_WritePos = 0; // 현재 저장위치
	m_ReadPos = 0;

	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}