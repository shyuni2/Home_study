#include "NetUser.h"

int NetUser::DispatchRead(char* RecvBuffer, int RecvByte)
{
	if (m_WritePos + RecvByte >= 2048)
	{
		if (m_ReadPos > 0)
		{
			memmove(&m_RecvBuffer[0], &m_RecvBuffer[m_PacketPos], m_ReadPos);
		}
		m_PacketPos = 0;
		m_WritePos = m_ReadPos;
	}

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
	m_cAddr = inet_ntoa(addr.sin_addr);
	m_Port = ntohs(addr.sin_port);
}