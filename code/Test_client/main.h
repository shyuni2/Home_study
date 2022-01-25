#pragma once
#include "AsyncSelect.h"
#include "Core.h"
#include "Window.h"

class main : public Core
{
	AsyncSelect m_Net;
	HWND m_hEdit;
	HWND m_hButton;
	HWND m_hUserChatBox;
	int m_MyChatBox;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	main();
	~main();
};

