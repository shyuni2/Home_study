#pragma once
#include <iostream>
#include <math.h>

struct Float2
{
	union
	{
		struct { float x, y; };
		float v[2];
	};
};

class Vector2 : public Float2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& v);
public: // 오버로딩
	Vector2 operator + (const Vector2& v);
	Vector2 operator - (const Vector2& v);
	Vector2 operator * (float fValue);
	Vector2 operator / (float fValue);
	Vector2 operator += (const Vector2& v);
	Vector2 operator -= (const Vector2& v);
	bool operator == (const Vector2& v);
	bool operator != (const Vector2& v);
public: // 정규화
	Vector2 Normalize();
	Vector2 Normal();
	friend Vector2 Normalize(Vector2& v);
public: 
	float Length(); // 크기
	float Distance(Vector2& v); // 거리
};

// ----------------------------------------------------------------------------------

struct Float4
{
	union
	{
		struct { float x, y, z, w; };
		float v[4];
	};
};

class Vector4 : public Float4
{
public:
	Vector4() 
	{
		v[0] = 0.0f;
		v[1] = 0.0f;
		v[2] = 0.0f;
		v[3] = 0.0f;
	};
	Vector4(float x, float y, float z, float w) 
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}
	Vector4(const Vector4& v) 
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
};