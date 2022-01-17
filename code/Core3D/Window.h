#pragma once
#include "Std.h"
class Window 
{
public:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	RECT m_rtClient;
	RECT m_rWindow;
public:
	BOOL SetWinClass(HINSTANCE hInstance);
	BOOL SeWindow(const WCHAR* szTitle = L"Sample", int iWidth = 800, int iHeight = 600);
	bool WinRun();
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Window();
	virtual ~Window();
};

