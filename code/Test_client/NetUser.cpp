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

}