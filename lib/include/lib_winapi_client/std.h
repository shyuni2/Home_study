#pragma once

#include <WinSock2.h> // Windows.h���� �����־�� ��
#include <Windows.h>

#include <iostream>

#include <list>
#include <vector>
#include <string>

using namespace std;

extern RECT m_rtWindow;
extern HWND g_hWnd;

#define WINRUN_START int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) { SampleCore;
