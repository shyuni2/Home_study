#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"

// �Լ�
LRESULT Sample::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 200:
		{
			char buffer[MAX_PATH] = { 0, };
			SendMessageA(m_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
			Packet sPacket(PACKET_CHAT_MSG);
			sPacket << 1 << "����" << (short)26 << buffer;
			m_Net.SendData(m_Net.m_Sock, sPacket.m_uPacket);

			SendMessageA(m_hEdit, WM_SETTEXT, 0, (LPARAM)" ");
		}break;
		}
	}break;
	}
	return m_Net.MsgProc(hWnd, msg, wParam, lParam);
}
bool Sample::Init()
{
	DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE ;
	m_hEdit = CreateWindow(L"edit", NULL, style, 10, g_rtClient.bottom - 60, 335, 50, m_hWnd, (HMENU)100, m_hInstance, NULL);
	style = WS_CHILD | WS_VISIBLE;
	m_hButton = CreateWindow(L"button", L"����", style, 355, g_rtClient.bottom - 60, 50, 50, m_hWnd, (HMENU)200, m_hInstance, NULL);
	m_hChatBox = CreateWindow(L"listbox", NULL, style, 10, 10, 395, g_rtClient.bottom - 70, m_hWnd, (HMENU)300, m_hInstance, NULL);

	m_hUserCount = CreateWindow(L"listbox", NULL, style, 418, 10, 140, 200, m_hWnd, (HMENU)400, m_hInstance, NULL);

	SendMessageA(m_hChatBox, LB_ADDSTRING, 0, (LPARAM)"ä�ù濡 �����ϼ̽��ϴ�");
	SendMessageA(m_hUserCount, LB_ADDSTRING, 0, (LPARAM)"���� ��������");

	
	m_Net.InitNetwork();
	m_Net.Connect(g_hWnd, SOCK_STREAM, 1, "192.168.0.87");
	return true;
}
bool	Sample::Frame()
{
	// ä��â
	int ChatBox = m_Net.m_ChatUser.m_PacketPool.size();
	if (ChatBox > 0 && m_ChatBox != ChatBox)
	{
		m_ChatBox = ChatBox;
		SendMessage(m_hChatBox, LB_RESETCONTENT, 0, 0);

		list<Packet>::iterator iter;
		if (m_Net.m_ChatUser.m_PacketPool.size() > 20)
		{
			m_Net.m_ChatUser.m_PacketPool.pop_front();
		}
		for (iter = m_Net.m_ChatUser.m_PacketPool.begin(); iter != m_Net.m_ChatUser.m_PacketPool.end(); iter++)
		{
			ChatMsg recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			(*iter) >> recvdata.index >> recvdata.name >> recvdata.age >> recvdata.message;
			SendMessageA(m_hChatBox, LB_ADDSTRING, 0, (LPARAM)recvdata.name); // �̸����
			SendMessageA(m_hChatBox, LB_ADDSTRING, 0, (LPARAM)recvdata.message); // �޼��� ���
			
			(*iter).Reset();
		}
	}

	// �������� ���
	ChatMsg recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	SendMessageA(m_hUserCount, LB_ADDSTRING, 0, (LPARAM)recvdata.name);

	return true;
}
bool Sample::Render()
{
	return true;
}
bool Sample::Release()
{	
	m_Net.CloseNetwork();
	return true;
}

// ������ �Ҹ���
Sample::Sample()
{

}
Sample::~Sample()
{

}

// ����
RUN();