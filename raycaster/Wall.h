#pragma once
#include <string>
#include "Config.h"
#include "Vector2.h"

class Wall
{
public:
	Vector2 pos;
	HBRUSH wall_color;
	bool render;
	Wall(Vector2 p, HBRUSH wcolor, bool render) :
		pos(p), wall_color(wcolor), render(render)
	{}
};
