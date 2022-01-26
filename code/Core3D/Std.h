#pragma once
#include <Winsock2.h> // 윈도우보다 윈속헤더가 위에 있어야함
#include <Windows.h>
#include <vector>
#include <list>
#include <d3d11.h>
#include "Collision.h"

#pragma comment (lib, "Core3D.lib")
#pragma comment	(lib, "d3d11.lib")
#pragma comment	(lib, "ws2_32.lib")

using namespace std;

extern RECT g_rtClient;
extern HWND g_hWnd;

#define GAME_START int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)  {main main;
#define SIMPLE_WIN() if (main.SetWinClass(hInstance) == FALSE) return 1; if (main.SetWindow() == FALSE) return 1; main.GameRun(); return 1;}
#define RUN() GAME_START; SIMPLE_WIN();
