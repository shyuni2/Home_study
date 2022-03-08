#pragma once
#include "Window.h"

// 1. ID3D11Device / ID3D11Device / IDXGISwapChain ���� �� ����
// 2. ID3D11RenderTargetView ���� �� ����
// 3. D3D11_VIEWPORT ����
// 4. ID3D11RenderTargetView ��ü�� �÷��� ä���(����۸� �����)
// 5. IDXGISwapChain ��ü�� ����Ͽ� �ÿ�����Ѵ�
// 6. ��� ������ ��ü���� �Ҹ� �Ǵ� ������Ų��

class Device : public Window
{
public:
	ID3D11Device* m_pd3dDevice;	
	ID3D11DeviceContext* m_pImmediateContext; // ��÷�����
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

