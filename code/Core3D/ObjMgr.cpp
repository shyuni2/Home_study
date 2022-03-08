#include "ObjMgr.h"
#include "Input.h"

void ObjMgr::AddCollisionExecute(BaseObj* owner, CollisionFunction func)
{
	owner->m_iCollisionID = m_iExcueteCollisionID++;
	m_ObjectList.insert(make_pair(owner->m_iCollisionID, owner));
	m_fnCollisionExecute.insert(make_pair(owner->m_iCollisionID, func));
}
void ObjMgr::DeleteCollisionExecute(BaseObj* owner)
{
	map<int, BaseObj*>::iterator objiter;
	objiter = m_ObjectList.find(owner->m_iCollisionID);
	if (objiter != m_ObjectList.end())
	{
		m_ObjectList.erase(objiter);
	}

	FuncionIterator colliter = m_fnCollisionExecute.find(owner->m_iCollisionID);
	if (colliter != m_fnCollisionExecute.end())
	{
		m_fnCollisionExecute.erase(colliter);
	}
}
void ObjMgr::AddSelectExecute(BaseObj* owner, CollisionFunction func)
{
	owner->m_iSelectID = m_iExcueteSelectID++;
	m_SelectList.insert(make_pair(owner->m_iSelectID, owner));
	m_fnSelectExecute.insert(make_pair(owner->m_iSelectID, func));
}
void ObjMgr::DeleteSelectExecute(BaseObj* owner)
{
	map<int, BaseObj*>::iterator objiter;
	objiter = m_SelectList.find(owner->m_iCollisionID);
	if (objiter != m_SelectList.end())
	{
		m_SelectList.erase(objiter);
	}

	FuncionIterator colliter = m_fnSelectExecute.find(owner->m_iCollisionID);
	if (colliter != m_fnSelectExecute.end())
	{
		m_fnSelectExecute.erase(colliter);
	}
}
bool ObjMgr::Init()
{
	return true;
}
bool ObjMgr::Frame()
{
	// collision
	for (auto src : m_ObjectList)
	{
		BaseObj* pObjSrc = (BaseObj*)src.second;
		if (pObjSrc->m_dwCollisonType == CollisionType::Ignore) continue;
		DWORD dwState = CollisionType::Overlap;
		for (auto dest : m_ObjectList)
		{
			BaseObj* pObjDest = (BaseObj*)dest.second;
			if (pObjSrc == pObjDest) continue;
			if (Collision::ToRect(pObjSrc->m_rtCollision, pObjDest->m_rtCollision))
			{
				FuncionIterator colliter = m_fnCollisionExecute.find(pObjSrc->m_iCollisionID);
				if (colliter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjDest, dwState);
				}
			}
		}
	}

	// mouse select
	for (auto src : m_SelectList)
	{
		BaseObj* pObjSrc = (BaseObj*)src.second;
		DWORD dwState = SelectState::T_DEFAULT;

		if (pObjSrc->m_dwSelectType != SelectType::Select_Ignore && Collision::RectToPoint(pObjSrc->m_rtCollision, (float)g_ptMouse.x, (float)g_ptMouse.y))
		{
			DWORD dwKeyState = Input::Get().m_dwMouseState[0];
			pObjSrc->m_dwSelectState = SelectState::T_HOVER;

			if (dwKeyState == KEY_PUSH)
			{
				pObjSrc->m_dwSelectState = SelectState::T_ACTIVE;
			}
			if (dwKeyState == KEY_HOLD)
			{
				pObjSrc->m_dwSelectState = SelectState::T_FOCUS;
			}
			if (dwKeyState == KEY_UP)
			{
				pObjSrc->m_dwSelectState = SelectState::T_SELECTED;
			}

			FuncionIterator colliter = m_fnSelectExecute.find(pObjSrc->m_iSelectID);
			if (colliter != m_fnSelectExecute.end())
			{
				CollisionFunction call = colliter->second;
				call(pObjSrc, dwState);
			}
		}
		else
		{
			if (pObjSrc->m_dwSelectState != SelectState::T_DEFAULT)
			{
				pObjSrc->m_dwSelectState = SelectState::T_DEFAULT;
				FuncionIterator colliter = m_fnSelectExecute.find(pObjSrc->m_iSelectID);
				if (colliter != m_fnSelectExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjSrc, dwState);
				}
			}
		}
	}
	return true;
}
bool ObjMgr::Release()
{
	m_ObjectList.clear();
	m_SelectList.clear();
	return true;
}

ObjMgr::ObjMgr()
{
	m_iExcueteCollisionID = 0;
	m_iExcueteSelectID = 0;
};
ObjMgr::~ObjMgr()
{
	Release();
};
