#include "Core.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

bool Core::CoreInit()
{
	m_GameTimer.Init();
	Input::Get().Init();
	if (SUCCEEDED(InitDeivice()))
	{
		I_Shader.Set(m_pd3dDevice);
		I_Texture.Set(m_pd3dDevice);
		DxState::SetState(m_pd3dDevice);
		if (m_dxWrite.Init())
		{
			IDXGISurface1* pSurface = nullptr;
			HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pSurface);
			if (SUCCEEDED(hr))
			{
				m_dxWrite.SetRenderTarget(pSurface);
			}
			if (pSurface) pSurface->Release();
		}
	}
	Init();

	return true;
}
bool Core::CoreFrame()
{
	m_GameTimer.Frame();
	Input::Get().Frame();
	I_ObjectMgr.Frame();
	I_Sound.Frame();
	Frame();
	m_dxWrite.Frame();
	return true;
}
bool Core::CoreRender()
{
	float color[4] = { 1,0,0,1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pImmediateContext->PSGetSamplers(0, 1, &DxState::m_pSamplerState);
	Render();
	m_GameTimer.Render();
	Input::Get().Render();
	m_dxWrite.Render();
	m_pSwapChain->Present(0, 0);
	return true;
}
bool Core::CoreRelease()
{
	Release();
	DxState::Release();
	m_dxWrite.Release();
	m_GameTimer.Release();
	Input::Get().Release();
	CleapupDevice();
	return true;
}

bool Core::GameRun()
{
	CoreInit();
	while (WinRun())
	{
		CoreFrame();
		CoreRender();
	}
	CoreRelease();
	MemoryReporting();
	return true;
}
void Core::ResizeDevice(UINT iWidth, UINT iHeight)
{
	if (m_pd3dDevice == nullptr) return;
	DeleteResizeDevice(iWidth, iHeight);
	m_dxWrite.DeleteDeviceResize();
	Device::ResizeDevice(iWidth, iHeight);
	IDXGISurface1* pSurface = nullptr;
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pSurface);
	if (SUCCEEDED(hr))
	{
		m_dxWrite.SetRenderTarget(pSurface);
	}
	if (pSurface) pSurface->Release();
	CreateResizeDevice(iWidth, iHeight);
}

Core::Core()
{

}
Core::~Core()
{

}