#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetUser.h"

int NetUser::DispatchRead(char* szRecvBuffer, int iRecvByte)
{
	// �����Ͱ� ©���� ���ü� �ֱ� ������ ��� �������� �𸣴ϱ� ������ġ�� �ٲ㼭 �� �޾��ش�
	if (m_iWritePos + iRecvByte >= 2048)
	{
		if (m_iReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos; // �߸� �������� �����Ҷ� �� ����
	}
	memcpy(&m_szRecvBuffer[m_iWritePos], szRecvBuffer, iRecvByte); // �����Ͱ� ��ŭ ���Դ��� Ȯ���Ҽ��ֱ� ������ ���� �縸ŭ �־���
	m_iWritePos += iRecvByte;// ���ۿ� ������ ����� ��ġ
	m_iReadPos += iRecvByte; // ��Ŷ���� ��ġ�κ��� ���� ����Ʈ

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// ��Ŷ �ؼ� ����
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// ��� 1���� ��Ŷ�� �����ߴ�.
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				Packet tPacket(pPacket->ph.type);
				memcpy(&tPacket.m_uPacket, &m_szRecvBuffer[m_iPacketPos], pPacket->ph.len); // �ϳ��� ��Ŷ �ϼ�
				m_PacketPool.push_back(tPacket); // �ϼ��� ��Ŷ Ǫ��

				// �ܿ��������� ������Ŷ ó��
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
	m_iWritePos = 0;
	m_iReadPos = 0;
	int m_iWritePos;  // ������ ���� ��ġ
	int m_iReadPos;

	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}