#include "BaseMgr.h"
#include <d3dcompiler.h>

class Shader
{
public:
	int m_iIndex;
	wstring m_csName;
	wstring m_szMsg;
	TCHAR m_szBuffer[256];
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3DBlob* m_pVSCodeResult = nullptr;
	ID3DBlob* m_pErrorMsgs = nullptr;
	ID3DBlob* m_pPSCodeResult = nullptr;
public:
	virtual bool CreateVertexShader(ID3D11Device* pd3dDevice, wstring filename, string entry = "VS");
	virtual bool CreatePixelShader(ID3D11Device* pd3dDevice, wstring filename, string entry = "PS");
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Load(ID3D11Device* pd3dDevice, wstring filename);
public:
	Shader();
	virtual ~Shader();
};

class ShaderMgr : public BaseMgr<Shader, ShaderMgr>
{
private:
	friend class Singleton<ShaderMgr>;
public:
	Shader* CreateVertexShader(ID3D11Device* pd3dDevice, wstring filename, string entry);
	Shader* CreatePixelShader(ID3D11Device* pd3dDevice, wstring filename, string entry);
private:
	ShaderMgr();
public:
	~ShaderMgr();
};

#define I_Shader ShaderMgr::Get()