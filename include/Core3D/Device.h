#pragma once
#include "Window.h"

// 1. ID3D11Device / ID3D11Device / IDXGISwapChain 선언 및 생성
// 2. ID3D11RenderTargetView 선언 및 생성
// 3. D3D11_VIEWPORT 설정
// 4. ID3D11RenderTargetView 객체에 컬러를 채운다(백버퍼를 지운다)
// 5. IDXGISwapChain 객체를 사용하여 시연출력한다
// 6. 모든 생성된 객체들을 소멸 또는 삭제시킨다

class Device : public Window
{
public:
	ID3D11Device* m_pd3dDevice;	
	ID3D11DeviceContext* m_pImmediateContext; // 즉시렌더링
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	D3D11_VIEWPORT m_ViewPort;
public:
	D3D_FEATURE_LEVEL m_FeatureLevel;	
	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;		
public:
	HRESULT InitDeivice();
	virtual bool CreateDevice();
	virtual bool CreateRenderTargetView();
	virtual bool SetViewport();
	virtual bool CleapupDevice();
public:
	void ResizeDevice(UINT iWidth, UINT iHeight);
public:
	Device();
	virtual ~Device();
};

