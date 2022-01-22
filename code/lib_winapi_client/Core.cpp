#include "Core.h"

bool Core::CoreInit()
{
	Init();
	return true;
}
bool Core::CoreFrame()
{
	Frame();
	return true;
}
bool Core::CoreRelease()
{
	Release();
	return true;
}
bool Core::CoreRender()
{
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // 색상값 설정
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	Render();

	m_pSwapChain->Present(0, 0);
	return true;
}

bool Core::ChatRun()
{
	return true;
}