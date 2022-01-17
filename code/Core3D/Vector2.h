#pragma once
#include <iostream>
#include <math.h>

struct TFloat2
{
	union
	{
		struct { float x, y; };
		float v[2];
	};
};
class Vector2 : public TFloat2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& v);
public:
	Vector2 operator + (const Vector2& v);
	Vector2 operator - (const Vector2& v);
	Vector2 operator * (float fValue);
	Vector2 operator / (float fValue);
	bool operator == (const Vector2& v);
	bool operator != (const Vector2& v);
	// 정규화
	Vector2 Normalize();
	Vector2 Normal();
	friend Vector2 Normalize(Vector2& v);
	// 크기
	float Length();
};

