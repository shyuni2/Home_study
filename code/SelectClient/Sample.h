#pragma once
#include "Core.h"
#include "AsyncSelect.h"

class Sample : public Core
{
public:
	AsyncSelect	m_Net;
	HWND m_hEdit; // 입력창
	HWND m_hButton; // 버튼
	HWND m_hChatBox; // 채팅창
	HWND m_hUserCount; // 접속유저리스트
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

