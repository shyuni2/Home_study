#pragma once
#include "Vector3.h"

enum CollisionType
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};
struct Rect
{
	Vector2 vMin;
	Vector2 vMax;
	Vector2 vMiddle;
	Vector2 size;
	bool operator == (const Rect& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Rect() {};
	Rect(Vector2 vMin, Vector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	Rect(Vector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + Vector2(w, h);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
struct Box
{
	Vector3 vMin;
	Vector3 vMax;
	Vector3 vMiddle;
	Vector3 size;
	bool operator == (const Box& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Box() {};
	Box(Vector3 vMin, Vector3 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		size.x = vMax.x - vMin.x;
		size.y = vMax.y - vMin.y;
	}
	Box(Vector3 v, float w, float h, float q)
	{
		this->vMin = v;
		this->vMax = vMin + Vector3(w, h,q);
		vMiddle = (vMax + vMin) / 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};

class Collision
{
public:
	static bool RectToPoint(Rect rt, int x, int y);
	static bool RectToPoint(Rect rt, Vector2 v);
	static CollisionType RectToRect(Rect,Rect);
	static Rect UnionRect(Rect rt1, Rect rt2);
	static bool IntersecRect(Rect rt1, Rect rt2, Rect* rt);
	
	static bool BoxToPoint(Box rt, int x, int y, int z);
	static bool BoxToPoint(Box rt, Vector3 v);
	static CollisionType BoxToBox(Box, Box);
	static Box UnionBox(Box rt1, Box rt2);
	static bool IntersecBox(Box rt1, Box rt2, Box* rt);
};

