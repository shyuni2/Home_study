#pragma once
#include "STD.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#pragma comment (lib,"fmode_vc.lib")

class Sound
{
public:
	int m_iIndex;
	wstring m_csName;
	FMOD::System* m_pSystem = nullptr;
	FMOD::Sound* m_pSound = nullptr;
	FMOD::Channel* m_pChannel = nullptr;
	float m_fVolume = 0.5f;
	wstring m_szMsg;
	TCHAR m_szBuffer[256];
public:
	void Set(FMOD::System* pSystem, wstring name, int iIndex);
	void Play(bool bLoop = false);
	void PlayEffect();
	void Stop();
	void Paused();
	void VolumeUp(float fVolume = 0.1f);
	void VolumeDown(float fVolume = 0.1f);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sound();
	virtual ~Sound();
};

// ---------------------------------------------------------------------

class SoundMgr :public Singleton<SoundMgr>
{
private:
	int m_iIndex;
public:
	friend class Singleton<SoundMgr>;
	FMOD::System* m_pSystem = nullptr;
	map<wstring, Sound*> m_list;
public:
	Sound* Load(string filename);
	Sound* GetPtr(wstring key);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
private:
	SoundMgr();
public:
	~SoundMgr();
};

#define I_Sound SoundMgr::Get()
