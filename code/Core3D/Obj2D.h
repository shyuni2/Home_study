#pragma once
#include "DxObj.h"

class Obj2D : public DxObj
{
public:
	RECT m_rtSource;
	RECT m_rtDraw;
	Vector4 m_vColor;
public:
	float m_fAlpha = 0.0f;
	bool m_bFadeIn = false;
	bool m_bFadeOut = false;
	virtual void FadeIn();
	virtual void FadeOut();
public:
	virtual bool SetVertexData() override;
	virtual bool SetIndexData() override;
	virtual bool Frame() override;
	bool Load(ID3D11Device* pd3dDevice, wstring filename) { return true; };
public:
	virtual void SetRectSouce(RECT rt);
	virtual void SetRectDraw(RECT rt);
	virtual void SetPosition(Vector2 vPos);
	virtual void AddPosition(Vector2 vPos);
	virtual void UpdateRectDraw(RECT rt);
public:
	virtual void Convert(Vector2 center, float fWidth, float fHeight, vector<SimpleVertex>& retList); // 화면 좌표 위치를 중점으로 NDC변환
	virtual void Convert(vector<SimpleVertex>& list, vector<SimpleVertex>& retList); // 화면좌표계를 NDC변환
	virtual void ConvertIndex(Vector2 center, float fWidth, float fHeight, vector<SimpleVertex>& retList); // 화면 좌표 위치를 중점으로 NDC변환
	virtual void ConvertIndex(vector<SimpleVertex>& list, vector<SimpleVertex>& retList); // 화면좌표계를 NDC변환
public:
	Obj2D();
	~Obj2D();
};

