#pragma once
#include <WinSock2.h> // ��������� ������ ������� ���� �־����
#include <d3d11.h>
#include <winsock2.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgidebug.h>
#include <tchar.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <atlconv.h> // A2W
#include "Collision.h" // math ���� ���ο���

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "d3d11.lib")

using namespace std;

extern RECT		g_rtClient;
extern HWND		g_hWnd;
extern float	g_fSecPerFrame;
extern float	g_fGameTimer;
extern POINT	g_ptMouse;

static wstring to_mw(const string& _src)
{
	USES_CONVERSION;
	return wstring(A2W(_src.c_str()));
};

static string to_wm(const wstring& _src)
{
	USES_CONVERSION;
	return string(W2A(_src.c_str()));
};

template<class T>
class Singleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

static void DisplayText(const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char buf[1024 + 256] = { 0, };
	vsprintf_s(buf, fmt, arg);
	OutputDebugStringA((char*)buf);
	va_end(arg);
}
static void MemoryReporting()
{
#if defined(DEBUG) | defined(_DEBUG)
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));
	IDXGIDebug* debug;
	GetDebugInterface(IID_PPV_ARGS(&debug));
	OutputDebugStringW(L"Starting Live Direct3D Object Dump:\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);
	OutputDebugStringW(L"Completed Live Direct3D Object Dump.\r\n");
	debug->Release();
#endif
}

#define GAME_START int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){   Sample core;   
#define GAME_WIN(s,x,y) if (core.SetWinClass(hInstance) == FALSE) return 1; if (core.SetWindow(L#s, x, y) == FALSE) return 1; core.GameRun(); return 1;}
#define SIMPLE_WIN() if (core.SetWinClass(hInstance) == FALSE) return 1; if (core.SetWindow() == FALSE) return 1; core.GameRun(); return 1;}
#define GAME_RUN(s,x,y) GAME_START; GAME_WIN(s,x,y);
#define RUN() GAME_START; SIMPLE_WIN();