#include "NetUser.h"

int NetUser::Recv()
{
	// 비동기 로드
	m_wsaRecvBuffer.len = sizeof(char) * 256;
	m_wsaRecvBuffer.buf = m_szRecv;
	m_ovRecv.type = 1000;
	DWORD dwRead;
	DWORD lpFlags = 0;
	BOOL ret = WSARecv(m_Sock, &m_wsaRecvBuffer, 1, &dwRead, &lpFlags, (WSAOVERLAPPED*)&m_ovRecv, nullptr);

	return 0;
}
int NetUser::Dispatch(DWORD dwTrans, TOV* tov)
{
	if (m_bConnect == false)
	{
		return 0;
	}
	if (tov->type == 1000)
	{
		if (!DispatchRecv(m_szRecv, dwTrans))
		{

		}
		Recv();
	}
	if (tov->type == 2000)
	{
		if (!DispatchSend(dwTrans))
		{

		}
	}

	return 1;
}
int NetUser::DispatchRecv(char* szRecvBuffer, int iRecvByte)
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
	m_iWritePos += iRecvByte;// 버퍼에 이전에 저장된 위치
	m_iReadPos += iRecvByte; // 패킷시작 위치로부터 받은 바이트

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// 패킷 해석 가능
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// 적어도 1개의 패킷은 도착했다.
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				Packet Packet(pPacket->ph.type);
				memcpy(&Packet.m_uPacket,
					&m_szRecvBuffer[m_iPacketPos],
					pPacket->ph.len);
				m_PacketPool.push_back(Packet);

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
int NetUser::DispatchSend(DWORD dwTrans)
{
	return 0;
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
	m_Name = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}

bool NetUser::DisConnect()
{
	closesocket(m_Sock);

	return true;
}

NetUser::NetUser()
{

}
NetUser::~NetUser()
{

}