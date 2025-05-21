#pragma once
#include <cmath>

class Vector2 {
private:
public:
	float x, y;
	Vector2()
		: x(0), y(0) {}
	Vector2(float x, float y)
		: x(x), y(y) {}
	Vector2(const Vector2& v)
		: x(v.x), y(v.y) {}
	Vector2 sub(const Vector2 &that)
	{ return Vector2(x - that.x, y - that.y); }
	Vector2 add(const Vector2 &that)
	{ return Vector2(x + that.x, y + that.y); }
	Vector2 mul(const Vector2 &that)
	{ return Vector2(x*that.x, y*that.y); }
	Vector2 mul(float f)
	{ return Vector2(x*f, y*f); }
	float length()
	{ return sqrt(x*x + y*y); }
	Vector2 norm()
	{ float l = length();
	if (l == 0) return Vector2(0, 0);
	return Vector2(x/l, y/l); }
	Vector2 scale(float f)
	{ return Vector2(x*f, y*f); }
	float dot(const Vector2 &that)
	{ return x*that.x + y*that.y; }
	Vector2 rot90()
	{ return Vector2(-y, x); }
	Vector2 rot_minus90()
	{ return Vector2(y, -x); }
	Vector2 lerp(const Vector2 &that, float t)
	{ return Vector2(x + t * (that.x - x), y + t * (that.y - y)); }
};

inline Vector2 from_angle(float angle)
{ return Vector2(cos(angle), sin(angle)); }

inline Vector2 zero()
{ return Vector2(0, 0); }