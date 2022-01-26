#pragma once
#include "Vector2.h"

struct Float3
{
	union
	{
		struct { float x, y, z; };
		float v[3];
	};
};
class Vector3 : public Float3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3& v);
public:
	Vector3 operator + (const Vector3& v);
	Vector3 operator - (const Vector3& v);
	Vector3 operator * (float fValue);
	Vector3 operator / (float fValue);
	bool operator == (const Vector3& v);
	bool operator != (const Vector3& v);
	// ¡§±‘»≠
	Vector3 Normalize();
	Vector3 Normal();
	friend Vector3 Normalize(Vector3& v);
	float Length();
};

