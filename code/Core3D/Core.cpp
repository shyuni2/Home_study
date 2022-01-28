#include "Core.h"

bool Core::Init()
{
	return true;
};
bool Core::Frame()
{
	return true;
};
bool Core::Render()
{
	return true;
};
bool Core::Release()
{
	return true;
};

// ÄÚ¾î
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
bool Core::CoreRender()
{	
	float color[4] = { 1.0f, 0.83f, 0.0f, 0.0f };
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

bool Core::GameRun()
{
	CoreInit();
	while (WinRun())
	{
		CoreFrame();
	}
	CoreRelease();
	return true;
}

Core::Core()
{

}
Core::~Core()
{

}