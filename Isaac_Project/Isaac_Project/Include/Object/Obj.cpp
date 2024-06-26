﻿#include "Obj.h"
#include "../Scene/Layer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"
#include "../Camera.h"
#include "../Collider/Collider.h"

// 초기화
list<CObj*> CObj::m_ObjList;


CObj::CObj()
	:m_pTexture(NULL)
{
}

CObj::CObj(const CObj& obj)
{
	*this = obj;

	if (m_pTexture) 
	{
		m_pTexture->AddRef();
	}

	m_ColliderList.clear();

	list<CCollider*> ::const_iterator iter;
	list<CCollider*> ::const_iterator iterEnd = obj.m_ColliderList.end();

	for (iter = obj.m_ColliderList.begin(); iter != iterEnd; ++iter) 
	{
		CCollider* pColl = (*iter)->Clone();

		pColl->SetObj(this); // 프로토타입은 그대로 있잖아

		m_ColliderList.push_back(pColl);
	}
}

CObj::~CObj()
{
	Safe_Release_VecList(m_ColliderList);
	SAFE_RELEASE(m_pTexture);
}


void CObj::Input(float fDeltaTime)
{
}

int CObj::Update(float fDeltaTime)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;) 
	{
		if (!(*iter)->GetEnable()) 
		{
			++iter;
			continue;
		}

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLive()) 
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else 
		{
			++iter;
		}
	}

	return 0;
}

int CObj::LateUpdate(float fDeltaTime)
{

	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fDeltaTime);

		if (!(*iter)->GetLive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
		{
			++iter;
		}
	}


	return 0;
}

void CObj::Collision(float fDeltaTime)
{
}

void CObj::Render(HDC hdc, float fDeltaTime)
{
	if (m_pTexture) 
	{
		POSITION tPos = m_tPos - m_tSize * m_tPivot;
		tPos -= GET_SINGLE(CCamera)->GetPos();

		BitBlt(hdc, tPos.x, tPos.y, m_tSize.x, m_tSize.y, m_pTexture->GetDC(), 0,0, SRCCOPY);
	}


	//TODO 충돌체 랜더링은 디버그모드 빌드에서만 출력되게끔 처리 필요
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(hdc,fDeltaTime);

		if (!(*iter)->GetLive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
		{
			++iter;
		}
	}

}

void CObj::SetTexture(CTexture* pTexture)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = pTexture;

	if (pTexture) pTexture->AddRef();
}

void CObj::SetTexture(const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
}

CObj* CObj::CreateCloneObj(const string& strPrototypeKey, const string& strTag, class CLayer* pLayer)
{
	CObj* pProto = CScene::FindPrototype(strPrototypeKey);

	if (!pProto) return NULL;

	CObj* pObj = pProto->Clone();

	pObj->SetTag(strTag);

	if (pLayer)
	{
		pLayer->AddObject(pObj);
	}

	AddObj(pObj);

	return pObj;
}

void CObj::AddObj(CObj* pObj)
{
	pObj->AddRef();
	m_ObjList.push_back(pObj);
}

CObj* CObj::FindObject(const string& strTag)
{
	list<CObj*> ::iterator iter;
	list<CObj*> ::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter) 
	{
		if ((*iter)->GetTag() == strTag) 
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

void CObj::EraseObj(CObj* pObj)
{
	list<CObj*> ::iterator iter;
	list<CObj*> ::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pObj)
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void CObj::EraseObj(const string& strTag)
{
	list<CObj*> ::iterator iter;
	list<CObj*> ::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void CObj::EraseObj()
{
	Safe_Release_VecList(m_ObjList);
}

