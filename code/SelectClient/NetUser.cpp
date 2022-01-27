#include "NetUser.h"

int NetUser::DispatchRead(char* RecvBuffer, int iRecvByte)
{
	if (m_WritePos + iRecvByte >= 2048)
	{
		if (m_ReadPos > 0)
		{
			memmove(&m_RecvBuffer[0], &m_RecvBuffer[m_PacketPos], m_ReadPos);
		}
		m_PacketPos = 0;
		m_WritePos = m_ReadPos;
	}
	memcpy(&m_RecvBuffer[m_WritePos], RecvBuffer, iRecvByte);
	m_WritePos += iRecvByte;
	m_ReadPos += iRecvByte; 

	if (m_ReadPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* pPacket = (UPACKET*)&m_RecvBuffer[m_PacketPos];
		if (pPacket->ph.len <= m_ReadPos)
		{
			do {
				Packet tPacket(pPacket->ph.type);
				memcpy(&tPacket.m_uPacket, &m_RecvBuffer[m_PacketPos], pPacket->ph.len);
				m_PacketPool.push_back(tPacket);

				// 다음패킷 처리
				m_PacketPos += pPacket->ph.len;
				m_ReadPos -= pPacket->ph.len;
				if (m_ReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_RecvBuffer[m_PacketPos];
			} while (pPacket->ph.len <= m_ReadPos);
		}
	}
	return 1;
}

void NetUser::set(SOCKET sock, SOCKADDR_IN addr)
{
	m_Connect = true;
	ZeroMemory(m_RecvBuffer, sizeof(char) * 2048);
	m_PacketPos = 0;
	m_WritePos = 0;
	m_ReadPos = 0;
	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_Port = ntohs(addr.sin_port);
}