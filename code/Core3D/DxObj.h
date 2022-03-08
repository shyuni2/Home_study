#pragma once
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "DxState.h"

enum CollisionType 
{ 
	Block = 0, 
	Overlap, 
	Ignore, 
};
enum SelectType 
{ 
	Select_Block = 0, 
	Select_Overlap, 
	Select_Ignore,
};
enum SelectState
{
	T_DEFAULT = 0,  // ���콺�� ���� ���� ��
	T_HOVER = 1,	// ���콺�� ���� ���� ��
	T_FOCUS = 2,	// �������¿��� ���� ��ư�� �ٸ� ���� ������ �� (���)
	T_ACTIVE = 4,	// ���콺 ���� ��ư ������ ���� ��
	T_SELECTED = 8, // ���� ���¿��� ���ʹ�ư ������ �� (����)
};

struct SimpleVertex
{
	Vector2 v;
	Vector2 t;
};
struct Index
{
	DWORD _0;
	DWORD _1;
	DWORD _2;
};
struct ConstantData
{
	Vector4 Color;
	Vector4 Timer;
};

// ----------------------------------------------------------------

class BaseObj
{
public:
	wstring m_csName;
	BaseObj* m_pParent = nullptr;
	bool m_bDead;
	int m_iCollisionID;
	int m_iSelectID;
	float m_fSpeed;
	Vector2	m_vPos;
	Vector2	m_vDirection;
	float m_fWidth;
	float m_fHeight;
	Rect m_rtCollision;
	DWORD m_dwCollisonType;
	DWORD m_dwSelectType;
	DWORD m_dwSelectState;
	DWORD m_dwPreSelectState;
	bool m_bSelect;
	bool m_bAlphaBlend;
public:
	virtual void HitOverlap(BaseObj* pObj, DWORD dwState);
	virtual void HitSelect(BaseObj* pObj, DWORD dwState);
	virtual void SetCollisionType(DWORD dwCollisionType, DWORD dwSelectType)
	{
		m_dwCollisonType = dwCollisionType;
		m_dwSelectType = dwSelectType;
	}
public:
	BaseObj()
	{
		m_bDead = false;
		m_bSelect = false;
		m_bAlphaBlend = true;
		m_dwSelectState = T_DEFAULT;
		m_iCollisionID = -1;
		m_iSelectID = -1;
		m_vDirection.x = 0.0f;
		m_vDirection.y = 0.0f;
		m_dwCollisonType = CollisionType::Ignore;
		m_dwSelectType = SelectType::Select_Ignore;
	}
};

// ----------------------------------------------------------------

class DxObj : public BaseObj
{
public:
	Texture* m_pColorTex = nullptr;
	Texture* m_pMaskTex = nullptr;
	Shader* m_pVShader = nullptr;
	Shader* m_pPShader = nullptr;
	D3D11_TEXTURE2D_DESC m_TextureDesc;
public:
	vector<SimpleVertex> m_InitScreenList;
	vector<SimpleVertex> m_VertexList;
	ID3D11Buffer* m_pVertexBuffer;
public:
	vector<DWORD> m_IndexList;
	ID3D11Buffer* m_pIndexBuffer;
public:
	ConstantData m_ConstantList;
	ID3D11Buffer* m_pConstantBuffer;
public:
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
public:
	void SetDevice(ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* m_pContext);
	virtual bool LoadTexture(const TCHAR* szColorFileName, const TCHAR* szMaskFileName);
	virtual bool SetVertexData();
	virtual bool SetIndexData();
	virtual bool SetConstantData();
public:
	virtual bool Create(ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* m_pContext, const TCHAR* szShaderFileName = nullptr, const TCHAR* szTextureFileName = nullptr, const TCHAR* szMaskFileName = nullptr);
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateConstantBuffer();
	virtual bool CreateVertexShader(const TCHAR* szFile);
	virtual bool CreatePixelShader(const TCHAR* szFile);
	virtual bool CreateInputLayout();
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	DxObj();
	~DxObj();
};

