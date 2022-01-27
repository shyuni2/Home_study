#include "NetUser.h"

int NetUser::DispatchRead(char* szRecvBuffer, int iRecvByte)
{
	if (m_iWritePos + iRecvByte >= 2048)
	{
		if (m_iReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}
	memcpy(&m_szRecvBuffer[m_iWritePos], szRecvBuffer, iRecvByte);
	m_iWritePos += iRecvByte;
	m_iReadPos += iRecvByte; 

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				Packet tPacket(pPacket->ph.type);
				memcpy(&tPacket.m_uPacket, &m_szRecvBuffer[m_iPacketPos], pPacket->ph.len);
				m_packetPool.push_back(tPacket);

				// 다음패킷 처리
				m_iPacketPos += pPacket->ph.len;
				m_iReadPos -= pPacket->ph.len;
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
			} while (pPacket->ph.len <= m_iReadPos);
		}
	}
	return 1;
}

void NetUser::set(SOCKET sock, SOCKADDR_IN addr)
{
	m_bConnect = true;
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_iPacketPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}