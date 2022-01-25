#pragma once
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <list>
#include <d3d11.h>

#pragma comment (lib, "Core3D.lib")
#pragma comment	(lib, "d3d11.lib")
#pragma comment	(lib, "ws2_32.lib")


using namespace std;

extern RECT g_rtClient;
extern HWND g_hWnd;

#define GAME_START int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)  {main main;
#define SIMPLE_WIN() if (main.SetWinClass(hInstance) == FALSE) return 1; if (main.SetWindow() == FALSE) return 1; main.GameRun(); return 1;}
#define RUN() GAME_START; SIMPLE_WIN();
