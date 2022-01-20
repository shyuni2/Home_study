#include "Window.h"

Window* pWindow = nullptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) // 운영체제에서 호출하는 함수
{
	pWindow->MsgProc(hWnd, msg, wParam, lParam);
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0); 
	}break;
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}
	return 0;
}
LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//LRESULT CALLBACK ChatEnterProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) // 채팅창에서 엔터 처리
//{
//
//}
// 1. 윈도우 클래스 등록
BOOL Window::SetWindowClass(HINSTANCE hinstance)
{
	m_hInstance = hinstance;
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASSW));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hinstance;
	wc.lpszClassName = L"lsh"; // 운영체제 이름
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	if (RegisterClass(&wc) == false)
	{
		return FALSE;
	}
	return TRUE;
}
// 2. 윈도우 생성
BOOL Window::SetWindow(const WCHAR* Title, int Width, int Height)
{
	RECT rect = { 0, 0, Width, Height };
	AdjustWindowRect(&rect, WS_OVERLAPPED, FALSE);
	m_hWnd = CreateWindow(L"lsh"/*운영체제 이름과 다르면 안됨*/, Title, WS_OVERLAPPEDWINDOW, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, m_hInstance, NULL);
	if (m_hWnd == NULL)
	{
		return FALSE;
	}
	GetWindowRect(m_hWnd, &m_rtWindow);

	ShowWindow(m_hWnd, SW_SHOW);
}
// 3. 메세지 프로시저 처리
bool Window::WinRun()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // 메세지가 있으면 무한반복
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

Window::Window()
{

}
Window::~Window()
{

}