﻿#pragma once
class CRef
{

protected:
	CRef();
	virtual ~CRef() = 0;

	int m_iRef;
	bool m_bEnable; // 객체를 활성 비활성화 하는 멤버
	bool m_bLife;


public:
	void AddRef() 
	{
		++m_iRef;
	}

	int Release() 
	{
		--m_iRef;

		if (m_iRef == 0) 
		{
			delete this;
			return 0;
		}
		return m_iRef;
	}

	void SetEnable(bool bEnable) 
	{
		m_bEnable = bEnable;
	}

	void Die() 
	{
		m_bLife = false;
	}

	bool GetEnable() const 
	{
		return m_bEnable;
	}

	bool GetLive() const 
	{
		return m_bLife;
	}

};

