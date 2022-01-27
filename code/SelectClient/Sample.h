#pragma once
#include "Core.h"
#include "TAsyncSelect.h"
class Sample : public Core
{
	TAsyncSelect		m_Net;
	HWND				m_hEdit;
	HWND				m_hButton;
	HWND				m_hListBox;
	int					m_iChatCnt;
public:
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};

