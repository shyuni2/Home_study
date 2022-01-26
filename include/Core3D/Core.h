#pragma once
#include "Device.h"

class Core : public Device
{
public:
	virtual bool	Init()
	{
		return true;
	};
	virtual bool	Frame()
	{
		return true;
	};
	virtual bool	Render()
	{
		return true;
	};
	virtual bool	Release()
	{
		return true;
	};

private:
	bool CoreInit();	
	bool CoreFrame();
	bool CoreRender();
	bool CoreRelease();

public:
	bool	GameRun();

public:
	Core();
	virtual ~Core();
};

