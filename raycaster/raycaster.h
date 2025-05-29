#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Player.h"
#include "WorldMap.h"

struct Ray_Hit {
	Vector2 hit_pos;
	float ray_len;
	bool hit;
	Ray_Hit(Vector2 v, float d, bool h) 
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
	void init();
	bool inside_map(int cell_x, int cell_y, int cols, int rows);
	Ray_Hit cast_ray(Vector2& p1, Vector2& p2, int cols, int rows);
	void clear_frame(Pixel *pixels, int screen_width, int screen_height);
	void draw_frame(Pixel* pixels, Player& player, int screen_width, int screen_height);
	void* render_frame(int screen_width, int screen_height);

	Player& playerInstance();
	WorldMap& worldmapInstance();

	Vector2 grid_size;
	Player player;
	WorldMap world_map;

	const float r = 5.0F;
	const Vector2 move_speed{0.1F, 0.1F};
	const float rotation_speed = 0.05F;
};
