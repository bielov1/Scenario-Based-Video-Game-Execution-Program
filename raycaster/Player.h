#pragma once
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

class Player {
private:
	const double FOV = M_PI/2;
public:
	Vector2 pos;
	float dir = 3*M_PI/2;
	const Vector2 move_speed{0.1F, 0.1F};
	const float rotation_speed = 0.05F;

	Player()
		: pos(zero()) {}


	void fov_range(Vector2& out_p1, Vector2& out_p2)
	{
		double l = tan(FOV*0.5);
		Vector2 p = pos.add(from_angle(dir));
		out_p1 = p.sub(pos).rot_minus90().add(p).scale(l);
		out_p2 = p.sub(pos).rot90().add(p).scale(l);
	}

	void set_posx(int x)
	{ pos.x = x; }

	void set_posy(int y)
	{ pos.y = y; }
};