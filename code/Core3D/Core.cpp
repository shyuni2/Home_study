#include "Core.h"
bool Core::CoreInit()
{
	//InitDeivice();
	Init();
	return true;
}
bool Core::GameRun()
{
	CoreInit();
	while(WinRun())
	{
		CoreFrame();
		//CoreRender();
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
	//float color[4] = { 1.1543f, 1.23421f, 1.4323f,1.0f };
	m_pImmediateContext->ClearRenderTargetView(
		m_pRenderTargetView,
		color);
	
	// 백버퍼에 랜더링 한다.
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