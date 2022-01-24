#pragma once
#include "STDserver.h"

class Window
{
public:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	RECT m_rtClient;
	RECT m_rWindow;

public:
	BOOL SetWinClass(HINSTANCE hInstance); // 윈도우 클래스 등록
	BOOL SeWindow(const WCHAR* szTitle = L"Sample", int iWidth = 800, int iHeight = 600); // 윈도우 생성
	bool WinRun();
};
