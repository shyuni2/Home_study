#pragma once
#include "Core.h"
#include "AsyncSelect.h"

class Sample : public Core
{
public:
	AsyncSelect	m_Net;
	HWND m_hEdit; // �Է�â
	HWND m_hButton; // ��ư
	HWND m_hChatBox; // ä��â
	HWND m_hUserCount; // ������������Ʈ
	int m_ChatBox;
	int m_UserBox;
public:
	virtual bool Init()  override;
	virtual bool Frame()  override;
	virtual bool Render()  override;
	virtual bool Release()  override;
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};

