﻿#pragma once

typedef struct _tagResolution 
{
	unsigned int iW;
	unsigned int iH;


	_tagResolution()
		:iW(0)
		, iH(0)
	{

	}

	_tagResolution(float _x, float _y)
		:iW(_x)
		,iH(_y)
	{
		
	}
}RESOLUTION, *PRESOLUTION;

typedef struct _tagPosition 
{
	float x, y;

	_tagPosition() :
		x(0.f),
		y(0.f) 
	{
	}

	_tagPosition(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}
	_tagPosition(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	_tagPosition(const POINT& pt)
	{
		x = pt.x;
		y = pt.y;
	}

	// ====== = =======

	void operator = (const _tagPosition& pos) 
	{
		x = pos.x;
		y = pos.y;
	}

	void operator = (const POINT& pt)  
	{
		x = pt.x;
		y = pt.y;
	}

	void operator = (float f[2]) // 2칸짜리 배열 넣어도 동작
	{
		x = f[0];
		y = f[1];
	}

	void operator += (const _tagPosition& pos) 
	{
		x += pos.x;
		y += pos.y;
	}

	void operator -= (const _tagPosition& pos) 
	{
		x -= pos.x;
		y -= pos.y;
	}

	// ====== + =======

	_tagPosition operator +(const _tagPosition& pos) const
	{
		_tagPosition tPos;
		tPos.x = x + pos.x;
		tPos.y = y + pos.y;
		return tPos;
	}

	_tagPosition operator +(const POINT& pt) const
	{
		_tagPosition tPos;
		tPos.x = x + pt.x;
		tPos.y = y + pt.y;
		return tPos;
	}

	_tagPosition operator +( float f[2]) const
	{
		_tagPosition tPos;
		tPos.x = x + f[0];
		tPos.y = y + f[1];
		return tPos;
	}

	_tagPosition operator +( float f) const
	{
		_tagPosition tPos;
		tPos.x = x + f;
		tPos.y = y + f;
		return tPos;
	}

	// ====== - =======

	_tagPosition operator -(const _tagPosition& pos) const
	{
		_tagPosition tPos;
		tPos.x = x - pos.x;
		tPos.y = y - pos.y;
		return tPos;
	}

	_tagPosition operator -(const POINT& pt) const
	{
		_tagPosition tPos;
		tPos.x = x - pt.x;
		tPos.y = y - pt.y;
		return tPos;
	}

	_tagPosition operator -( float f[2]) const
	{
		_tagPosition tPos;
		tPos.x = x - f[0];
		tPos.y = y - f[1];
		return tPos;
	}

	_tagPosition operator -( float f) const
	{
		_tagPosition tPos;
		tPos.x = x - f;
		tPos.y = y - f;
		return tPos;
	}

	// ====== * =======


	_tagPosition operator *(const _tagPosition& pos) const
	{
		_tagPosition tPos;
		tPos.x = x * pos.x;
		tPos.y = y * pos.y;
		return tPos;
	}

	_tagPosition operator *(const POINT& pt) const
	{
		_tagPosition tPos;
		tPos.x = x * pt.x;
		tPos.y = y * pt.y;
		return tPos;
	}

	_tagPosition operator *( float f[2]) const
	{
		_tagPosition tPos;
		tPos.x = x * f[0];
		tPos.y = y * f[1];
		return tPos;
	}

	_tagPosition operator *(float f) const
	{
		_tagPosition tPos;
		tPos.x = x * f;
		tPos.y = y * f;
		return tPos;
	}
	
	// ===== / ======

	_tagPosition operator /(const _tagPosition& pos) const
	{
		_tagPosition tPos;
		tPos.x = x / pos.x;
		tPos.y = y / pos.y;
		return tPos;
	}

	_tagPosition operator /(const POINT& pt) const
	{
		_tagPosition tPos;
		tPos.x = x / pt.x;
		tPos.y = y / pt.y;
		return tPos;
	}

	_tagPosition operator /( float f[2]) const
	{
		_tagPosition tPos;
		tPos.x = x / f[0];
		tPos.y = y / f[1];
		return tPos;
	}

	_tagPosition operator /( float f) const
	{
		_tagPosition tPos;
		tPos.x = x / f;
		tPos.y = y / f;
		return tPos;
	}

} POSITION, *PPOSITION, _SIZE, *_PSIZE;


typedef struct _tagRectangle 
{
	float l;
	float t;
	float r;
	float b;

	_tagRectangle()
		:l(0.f)
		,t(0.f)
		,r(0.f)
		,b(0.f)
	{
	}

}RECTANGLE, *PRECTANGLE;