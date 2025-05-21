#pragma once
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

class Player {
private:
	const float FOV = M_PI/2;
public:
	Vector2 pos;
	float dir;
	int hp = 0;
	std::string moving;

	Player(Vector2 p, float d)
		: pos(p), dir(d) {}
	Player()
		: pos(zero()), dir(0.0F) {}
	void init(int hp, Vector2 pos, float dir, std::string mov)
	{
		this->hp = hp;
		this->pos = pos;
		this->dir = dir;
		this->moving = mov;
	}
	void fov_range(Vector2& out_p1, Vector2& out_p2)
	{
		float l = tan(FOV*0.5);
		Vector2 p = pos.add(from_angle(dir));
		out_p1 = p.sub(pos).rot_minus90().add(p).scale(l);
		out_p2 = p.sub(pos).rot90().add(p).scale(l);
	}

};