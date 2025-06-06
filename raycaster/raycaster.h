#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Player.h"
#include "WorldMap.h"


struct Ray_Hit {
	Vector2 hit_pos;
	double ray_len;
	bool hit;
	Ray_Hit(Vector2 v, double d, bool h) 
		: hit_pos(v), ray_len(d), hit(h) {}

	Ray_Hit()
		: hit_pos(Vector2(0, 0)), ray_len(0.0F), hit(false) {}
};

struct Pixel {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

typedef Pixel* Pixels;


class Player;

class Raycaster
{
public:
	Raycaster();
	void init(Player& player, int map_width, int map_height);
	bool inside_map(int cell_x, int cell_y, int cols, int rows);
	Ray_Hit cast_ray(WorldMap& world_map, Vector2& p1, Vector2& p2, int cols, int rows);
	void clear_frame(Pixel *pixels, int screen_width, int screen_height);
	void draw_frame(WorldMap& world_map, Pixel* pixels, Player& player, int screen_width, int screen_height);
	void* render_frame(WorldMap& world_map, Player& player, int screen_width, int screen_height);

	Vector2 grid_size;
};
