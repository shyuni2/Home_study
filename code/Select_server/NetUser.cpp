#include "NetUser.h"

int NetUser::DispatchRead(char* szRecvBuffer, int iRecvByte)
{
	// �����Ͱ� ©���� ���ü� �ֱ� ������ ������ġ�� �ٲ㼭 �� ����
	if (m_iWritePos + iRecvByte >= 2048)
	{
		if (m_iReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos; // �߸� ¥������ ������ �� ����
	}
	// ��ŭ �����Ͱ� ���Դ��� Ȯ�� �� �� �ֱ� ������ ���� �� ��ŭ ����
	memcpy(&m_szRecvBuffer[m_iWritePos], szRecvBuffer, iRecvByte);
	m_iWritePos += iRecvByte;// ���ۿ� ������ ����� ��ġ
	m_iReadPos += iRecvByte; // ��Ŷ���� ��ġ�κ��� ���� ����Ʈ

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// ��Ŷ �ؼ� ����
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos]; // ��絥���Ͱ� ���ڿ��� ������ ������ ���, ó���� 0���ּҰ� ��Ŷ
		// ��� 1���� ��Ŷ�� �����ߴ�.
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				Packet Packet(pPacket->ph.type);
				memcpy(&Packet.m_uPacket, &m_szRecvBuffer[m_iPacketPos], pPacket->ph.len); // ��Ŷ �ϼ�
				m_PacketPool.push_back(Packet); // �ϼ���Ŷ Ǫ��

				// ������Ŷ ó��, �ܿ��� ������ �� ������ ����
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
	m_bConnect = true; // ���� ����
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_iPacketPos = 0;
	m_iWritePos = 0; // ���� ���� ��ġ
	m_iReadPos = 0;

	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}