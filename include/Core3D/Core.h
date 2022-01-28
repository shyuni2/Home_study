#pragma once
#include "Device.h"

class Core : public Device
{
private:
	bool CoreInit();	
	bool CoreFrame();
	bool CoreRender();
	bool CoreRelease();
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	bool GameRun();
public:
	Core();
	virtual ~Core();
};

