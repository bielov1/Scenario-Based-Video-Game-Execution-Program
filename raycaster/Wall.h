#pragma once
#include <Windows.h>
#include <string>
#include "Vector2.h"

class Wall
{
public:
	Vector2 pos;
	COLORREF rgb_color;
	bool render;
	Wall(Vector2 p,  COLORREF rgb, bool render) :
		pos(p),  rgb_color(rgb), render(render)
	{}
	COLORREF get_rgb_color()
	{ return rgb_color; }
};
