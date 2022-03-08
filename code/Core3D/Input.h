#pragma once
#include "STD.h"

enum KeyState { KEY_FREE = 0, KEY_UP, KEY_PUSH, KEY_HOLD, };

class Input
{
public:
	DWORD m_dwKeyState[256];
	POINT m_ptMouse;
	DWORD m_dwMouseState[3];
	static Input& Get()
	{
		static Input theSingle;
		return theSingle;
	}
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	DWORD GetKey(DWORD dwKey);
private:
	Input();
public: 
	virtual ~Input();
};

