#pragma once
#include "StaticObj.h"
class CStage :
    public CStaticObj
{
public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hdc, float fDeltaTime);
	virtual CStaticObj* Clone();

private:
	friend class CObj;
	friend class CScene;

	CStage();
	CStage(const CStage& stage);
    ~CStage();
};

