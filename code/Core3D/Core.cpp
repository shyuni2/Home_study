#include "Core.h"

bool Core::CoreInit()
{
	Init();
	return true;
}
bool Core::GameRun()
{
	CoreInit();
	while(WinRun())
	{
		CoreFrame();
	}
	CoreRelease();
	return true;
}
bool Core::CoreFrame()
{
	Frame();
	return true;
}
bool Core::CoreRender()
{	
	float color[4] = { 0.1543f, 0.23421f, 0.4323f,1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
	
	Render();
	m_pSwapChain->Present(0, 0);
	return true;
}

bool Core::CoreRelease()
{
	Release();
	CleapupDevice();
	return true;
}

Core::Core()
{

}
Core::~Core()
{

}