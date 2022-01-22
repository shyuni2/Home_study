#include "Device.h"

HRESULT Device::InitDeivice()
{
	HRESULT hr = S_OK;
	CreateDevice();
	CreateRenderTargetView();
	SetViewport();
	return hr;
}
bool Device::CreateDevice()
{
	UINT Flags = 0;
	D3D_FEATURE_LEVEL fl[]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};

	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	m_SwapChainDesc.BufferDesc.Width = m_rtWindow.right;
	m_SwapChainDesc.BufferDesc.Height = m_rtWindow.bottom;
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.OutputWindow = m_hWnd;
	m_SwapChainDesc.Windowed = true;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, Flags, fl, 1, D3D11_SDK_VERSION, &m_SwapChainDesc, &m_pSwapChain, &m_pD3DDevice, &m_FeatureLevel, &m_pDeviceContext);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool Device::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	m_pD3DDevice->CreateRenderTargetView(backBuffer, NULL, &m_pRenderTargetView);
	if (backBuffer)backBuffer->Release();

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	return true;
}
bool Device::SetViewport()
{
	// ����Ʈ ����
	//DXGI_SWAP_CHAIN_DESC swapDesc;
	//m_pSwapChain->GetDesc(&swapDesc);

	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = m_SwapChainDesc.BufferDesc.Width;
	m_ViewPort.Height = m_SwapChainDesc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1, &m_ViewPort);
	return true;
}
bool Device::CleapupDevice()
{
	if (m_pD3DDevice)m_pD3DDevice->Release();	// ����̽� ��ü
	if (m_pDeviceContext)m_pDeviceContext->Release();// �ٺ��̽� ���ؽ�Ʈ ��ü
	if (m_pSwapChain)m_pSwapChain->Release();	// ����ü�� ��ü
	if (m_pRenderTargetView)m_pRenderTargetView->Release();
	m_pD3DDevice = nullptr;	// ����̽� ��ü
	m_pDeviceContext = nullptr;// �ٺ��̽� ���ؽ�Ʈ ��ü
	m_pSwapChain = nullptr;	// ����ü�� ��ü
	m_pRenderTargetView = nullptr;
	return true;
}
Device::Device()
{
	m_pD3DDevice = nullptr;	// ����̽� ��ü
	m_pDeviceContext = nullptr;// �ٺ��̽� ���ؽ�Ʈ ��ü
	m_pSwapChain = nullptr;	// ����ü�� ��ü
	m_pRenderTargetView = nullptr;
}
Device::~Device()
{

}