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
	BOOL SetWinClass(HINSTANCE hInstance); // ������ Ŭ���� ���
	BOOL SeWindow(const WCHAR* szTitle = L"Sample", int iWidth = 800, int iHeight = 600); // ������ ����
	bool WinRun();
};
