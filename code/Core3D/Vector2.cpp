#include "Vector2.h"

Vector2::Vector2()
{
	x = y = 0.0f;
}
Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}
Vector2::Vector2(const Vector2& v)
{
	x = v.x;
	y = v.y;
}

// 벡터연산
Vector2 Vector2::operator + (const Vector2& v)
{
	Vector2 ret;
	ret.x = this->x + v.x;
	ret.y = this->y + v.y;	
	return ret;
}
Vector2 Vector2::operator - (const Vector2& v)
{
	Vector2 ret;
	ret.x = this->x - v.x;
	ret.y = this->y - v.y;
	return ret;
}
Vector2 Vector2::operator * (float fValue)
{
	this->x *= fValue;
	this->y *= fValue;
	return *this;
}
Vector2 Vector2::operator / (float fValue)
{
	this->x /= fValue;
	this->y /= fValue;
	return *this;
}
bool Vector2::operator == (const Vector2& v)
{
	if (fabs(x - v.x) < 0.0001f)
	{
		if (fabs(y - v.y) < 0.0001f)
		{
			return true;
		}
	}
	return false;
}
bool Vector2::operator != (const Vector2& v)
{
	if (fabs(x - v.x) < 0.0001f)
	{
		if (fabs(y - v.y) < 0.0001f)
		{
			return false;
		}
	}
	return true;
}

// 정규화
Vector2 Vector2::Normal()
{
	Vector2 ret;
	float length = 1.0f / Length();
	ret.x = x * length;
	ret.y = y * length;
	return ret;
}
Vector2 Vector2::Normalize()
{
	float length = 1.0f / Length();
	x *= length;
	y *= length;
	return *this;
}
Vector2 Normalize( Vector2& v)
{
	Vector2 ret;
	float length = 1.0f / v.Length();
	ret.x = v.x * length;
	ret.y = v.y * length;
	return ret;
}
float Vector2::Length()
{
	float ret = x * x + y * y;
	return sqrt(ret);
}