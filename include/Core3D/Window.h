#pragma once
#include "STD.h"

// 1. 윈도우 클래스 등록, 허가
// 2. 윈도우 생성
// 3. 메세지 프로시저 처리

class Window
{
public:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	RECT m_rtClient;
	RECT m_rtWindow;
public:
	BOOL SetWinClass(HINSTANCE hInstance); // 1
	BOOL SetWindow(const WCHAR* szTitle = L"Sample", int iWidth = 800, int iHeight = 600); // 2
	bool WinRun();
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void     ResizeDevice(UINT iWidth, UINT iHeight);
public:
	Window();
	virtual ~Window();
};