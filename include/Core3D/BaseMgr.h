#pragma once
#include "STD.h"

template<class T, class S>
class BaseMgr: public Singleton<S>
{
public:
	friend class Singleton<BaseMgr>;
public:
	int m_iIndex;
	ID3D11Device* m_pd3dDevice;
	map<wstring, T* >  m_list;
public:
	T* GetPtr(wstring key);
	wstring Splitpath(wstring path, wstring entry);
	virtual void Set(ID3D11Device* pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
	}
	virtual T* Load(wstring filename);	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	BaseMgr();
	~BaseMgr();
};

template<class T, class S>
wstring BaseMgr<T, S>::Splitpath(wstring path,	wstring entry)
{
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };

	wstring fullpathname = path;
	_tsplitpath_s(fullpathname.c_str(), Dirve, Dir, FileName, FileExt);
	wstring name = FileName;
	name += FileExt;
	if (entry.empty() == false)
	{
		name += entry;
	}
	return name;
}

template<class T, class S>
T* BaseMgr<T, S>::GetPtr(wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second;
	}
	return nullptr;
}

template<class T, class S>
T* BaseMgr<T, S>::Load(wstring filename)
{
	wstring name = Splitpath(filename, L"");
	T* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new T;
	if (pData->Load(m_pd3dDevice, filename) == false)
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}

template<class T, class S>
bool BaseMgr<T, S>::Init()
{
	return true;
}

template<class T, class S>
bool BaseMgr<T, S>::Frame()
{
	return true;
}

template<class T, class S>
bool BaseMgr<T, S>::Render()
{
	return true;
}

template<class T, class S>
bool BaseMgr<T, S>::Release()
{
	for (auto data : m_list)
	{
		data.second->Release();
		delete data.second;
	}
	m_list.clear();
	return true;
}

template<class T, class S>
BaseMgr<T, S>::BaseMgr()
{
	m_iIndex = 0;
}
template<class T, class S>
BaseMgr<T, S>::~BaseMgr()
{
	Release();
}