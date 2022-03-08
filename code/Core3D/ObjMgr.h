#pragma once
#include "Obj2D.h"

using CollisionFunction = function<void(BaseObj*, DWORD)>;
using SelectFunction = function<void(BaseObj*, DWORD)>;

class ObjMgr : public Singleton< ObjMgr>
{
private:
	int m_iExcueteCollisionID;
	int m_iExcueteSelectID;
	map<int, BaseObj*> m_ObjectList;
	map<int, BaseObj*> m_SelectList;
public:
	friend Singleton< ObjMgr>;
public:
	typedef map<int, CollisionFunction>::iterator FuncionIterator;
	map<int, CollisionFunction> m_fnCollisionExecute;
	typedef map<int, SelectFunction>::iterator FuncionIterator;
	map<int, SelectFunction> m_fnSelectExecute;
public:
	void  AddCollisionExecute(BaseObj* owner, CollisionFunction func);
	void  DeleteCollisionExecute(BaseObj* owner);
	void  AddSelectExecute(BaseObj* owner, CollisionFunction func);
	void  DeleteSelectExecute(BaseObj* owner);
public:
	bool  Init();
	bool  Frame();
	bool  Release();
private:
	ObjMgr();
public:
	virtual ~ObjMgr();
};

#define I_ObjectMgr   ObjMgr::Get()
