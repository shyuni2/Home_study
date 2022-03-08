#include "ShaderMgr.h"

bool Shader::CreateVertexShader(ID3D11Device* pd3dDevice, wstring filename, string entry)
{
	HRESULT hr = D3DCompileFromFile(filename.c_str(), NULL, NULL, entry.c_str(), "vs_5_0", 0, 0, &m_pVSCodeResult, &m_pErrorMsgs);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)m_pErrorMsgs->GetBufferPointer(), "ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = pd3dDevice->CreateVertexShader(m_pVSCodeResult->GetBufferPointer(), m_pVSCodeResult->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr)) { return false; }
	return true;
}
bool Shader::CreatePixelShader(ID3D11Device* pd3dDevice, wstring filename, string entry)
{
	HRESULT hr = D3DCompileFromFile(filename.c_str(), NULL, NULL, entry.c_str(), "ps_5_0", 0, 0, &m_pPSCodeResult, &m_pErrorMsgs);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)m_pErrorMsgs->GetBufferPointer(), "ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = pd3dDevice->CreatePixelShader(m_pPSCodeResult->GetBufferPointer(), m_pPSCodeResult->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(hr))
	{ 
		return false;
	}
	return true;
}
bool Shader::Load(ID3D11Device* pd3dDevice, wstring filename)
{
	if (!CreateVertexShader(pd3dDevice, filename))
	{
		return false;
	}
	if (!CreatePixelShader(pd3dDevice, filename))
	{ 
		return false;
	}
	return true;
}
bool Shader::Init() 
{
	return true;
}
bool Shader::Frame()
{
	return true;
}
bool Shader::Render()
{
	return true;
}
bool Shader::Release()
{
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	return true;
}

Shader::Shader()
{

}
Shader::~Shader()
{

}

// ------------------------------------------------------------

Shader* ShaderMgr::CreateVertexShader(ID3D11Device* pd3dDevice, wstring filename, string entry)
{
	wstring name = Splitpath(filename, to_mw(entry));
	Shader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new Shader;
	if (!pData->CreateVertexShader(pd3dDevice, filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
Shader* ShaderMgr::CreatePixelShader(ID3D11Device* pd3dDevice, wstring filename, string entry)
{
	wstring name = Splitpath(filename, to_mw(entry));
	Shader* pData = GetPtr(name);
	if (pData != nullptr) { return pData; }
	pData = new Shader;
	if (!pData->CreatePixelShader(pd3dDevice, filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
ShaderMgr::ShaderMgr()
{
	m_iIndex = 0;
}
ShaderMgr::~ShaderMgr()
{
	Release();
}