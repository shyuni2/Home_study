#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetUser.h"

int NetUser::DispatchRead(char* RecvBuffer, int RecvByte)
{
	// �����Ͱ� ©���� ���ü� �ֱ� ������ ��� �������� �𸣴ϱ� ������ġ�� �ٲ㼭 �� �޾��ش�
	if (m_WritePos + RecvByte >= 2048)
	{
		if (m_ReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_PacketPos], m_ReadPos);
		}
		m_PacketPos = 0;
		m_WritePos = m_ReadPos; // �߸� �������� �����Ҷ� �� ����
	}
	memcpy(&m_szRecvBuffer[m_WritePos], RecvBuffer, RecvByte); // �����Ͱ� ��ŭ ���Դ��� Ȯ���Ҽ��ֱ� ������ ���� �縸ŭ �־���
	m_WritePos += RecvByte;// ���ۿ� ������ ����� ��ġ
	m_ReadPos += RecvByte; // ��Ŷ���� ��ġ�κ��� ���� ����Ʈ

	if (m_ReadPos >= PACKET_HEADER_SIZE)
	{
		// ��Ŷ �ؼ� ����
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_PacketPos];
		// ��� 1���� ��Ŷ�� �����ߴ�.
		if (pPacket->ph.len <= m_ReadPos)
		{
			do {
				Packet tPacket(pPacket->ph.type);
				memcpy(&tPacket.m_uPacket, &m_szRecvBuffer[m_PacketPos], pPacket->ph.len); // �ϳ��� ��Ŷ �ϼ�
				m_PacketPool.push_back(tPacket); // �ϼ��� ��Ŷ Ǫ��

				// �ܿ��������� ������Ŷ ó��
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
	m_bConnect = true; // ���� ����
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_PacketPos = 0;
	m_WritePos = 0;
	m_ReadPos = 0;
	int m_WritePos;  // ������ ���� ��ġ
	int m_ReadPos;

	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}