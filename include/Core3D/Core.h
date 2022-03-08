#pragma once
#include "Device.h"
#include "Input.h"
#include "DxObj.h"
#include "Timer.h"
#include "WriteFont.h"

class Core : public Device
{
public:
	Timer m_GameTimer;
	WriteFont  m_dxWrite;
public:
	ID3D11SamplerState* m_pSamplerState = nullptr;
private:
	bool CoreInit();
	bool CoreFrame();
	bool CoreRender();
	bool CoreRelease();
public:
	bool GameRun();
	void ResizeDevice(UINT iWidth, UINT iHeight);
	virtual void CreateResizeDevice(UINT iWidth, UINT iHeight) {};
	virtual void DeleteResizeDevice(UINT iWidth, UINT iHeight) {};
public:
	virtual bool Init() { return true; }
	virtual bool Frame() { return true; }
	virtual bool Render() { return true; }
	virtual bool Release() { return true; }
public:
	Core();
	virtual ~Core();
};

