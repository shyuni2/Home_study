#pragma once
#include "Core.h"
#include "AsyncSelect.h"

class Sample : public Core
{
public:
	AsyncSelect	m_Net;
	HWND m_hEdit;
	HWND m_hButton;
	HWND m_hListBox;
	int m_ChatCnt;
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

