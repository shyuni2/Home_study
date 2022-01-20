#pragma once
#include "std.h"

class Window
{
public:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	RECT m_rtWindow;
public:
	BOOL SetWindowClass(HINSTANCE hinstance);
	BOOL SetWindow(const WCHAR* Title = L"text", int Width = 800, int Height = 400);
	bool WinRun();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Window();
	virtual ~Window();
};

