#include "Window.h"

Window* pWindow = nullptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) // �ü������ ȣ���ϴ� �Լ�
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
//LRESULT CALLBACK ChatEnterProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) // ä��â���� ���� ó��
//{
//
//}
// 1. ������ Ŭ���� ���
BOOL Window::SetWindowClass(HINSTANCE hinstance)
{
	m_hInstance = hinstance;
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASSW));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hinstance;
	wc.lpszClassName = L"lsh"; // �ü�� �̸�
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	if (RegisterClass(&wc) == false)
	{
		return FALSE;
	}
	return TRUE;
}
// 2. ������ ����
BOOL Window::SetWindow(const WCHAR* Title, int Width, int Height)
{
	RECT rect = { 0, 0, Width, Height };
	AdjustWindowRect(&rect, WS_OVERLAPPED, FALSE);
	m_hWnd = CreateWindow(L"lsh"/*�ü�� �̸��� �ٸ��� �ȵ�*/, Title, WS_OVERLAPPEDWINDOW, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, m_hInstance, NULL);
	if (m_hWnd == NULL)
	{
		return FALSE;
	}
	GetWindowRect(m_hWnd, &m_rtWindow);

	ShowWindow(m_hWnd, SW_SHOW);
}
// 3. �޼��� ���ν��� ó��
bool Window::WinRun()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // �޼����� ������ ���ѹݺ�
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