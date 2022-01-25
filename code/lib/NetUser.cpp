#include "NetUser.h"

// 패킷전송
int NetUser::SendData(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize],
			packet.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.ph.len);
	return iSendSize;
}
int NetUser::SendData(SOCKET sock, char* msg, WORD type)
{
	Packet tPacket(type);
	tPacket << 999 << "이승현" << (short)50 << msg;
	Packet tPacketTest(tPacket);
	ChatMsg recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	tPacketTest >> recvdata.index >> recvdata.name >> recvdata.damage >> recvdata.message;
	char* pData = (char*)&tPacket.m_uPacket;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pData[SendSize], tPacket.m_uPacket.ph.len - SendSize, 0);
		if (SendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		SendSize += SendByte;
	} while (SendSize < tPacket.m_uPacket.ph.len);
	return SendSize;
}
int NetUser::RecvPacketHeader(SOCKET sock, UPACKET& recvPacket)
{
	char RecvBuffer[256] = { 0, };
	//패킷헤더 받기
	ZeroMemory(&recvPacket, sizeof(recvPacket));
	bool bRun = true;
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, RecvBuffer, PACKET_HEADER_SIZE, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << " 접속종료됨." << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				cout << " 비정상 접속종료됨." << endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (RecvSize < PACKET_HEADER_SIZE);
	memcpy(&recvPacket.ph, RecvBuffer, PACKET_HEADER_SIZE);
	return 1;
}
int NetUser::RecvData(NetUser& user)
{
	char szRecvBuffer[1024] = { 0, };
	int iRecvByte = recv(user.m_Sock, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return 2;
	}
	user.DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}

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
	memcpy(&m_RecvBuffer[m_WritePos], RecvBuffer, RecvByte);
	m_WritePos += RecvByte;// 버퍼에 이전에 저장된 위치
	m_ReadPos += RecvByte; // 패킷시작 위치로부터 받은 바이트

	if (m_ReadPos >= PACKET_HEADER_SIZE)
	{
		// 패킷 해석 가능
		UPACKET* pPacket = (UPACKET*)&m_RecvBuffer[m_PacketPos];
		// 적어도 1개의 패킷은 도착했다.
		if (pPacket->ph.len <= m_ReadPos)
		{
			do {
				Packet tPacket(pPacket->ph.type);
				memcpy(&tPacket.m_uPacket, &m_RecvBuffer[m_PacketPos], pPacket->ph.len);
				m_packetPool.push_back(tPacket);

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

int NetUser::Broadcast(NetUser& user)
{
	if (user.m_packetPool.size() > 0)
	{
		std::list<Packet>::iterator iter;
		for (iter = user.m_packetPool.begin(); iter != user.m_packetPool.end(); )
		{
			for (NetUser& senduser : m_Userlist)
			{
				int ret = SendData(senduser.m_Sock, (*iter).m_uPacket);
				if (ret <= 0)
				{
					senduser.m_Connect = false;
				}
			}
			iter = user.m_packetPool.erase(iter);
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


