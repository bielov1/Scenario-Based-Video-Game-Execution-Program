#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Config.h"
#include "Player.h"
#include "WorldMap.h"

#define screen_width    800
#define screen_height   600

struct Ray_Hit {
	Vector2 hit_pos;
	float ray_len;
	bool hit;
	Ray_Hit(Vector2 v, float d, bool h) 
		: hit_pos(v), ray_len(d), hit(h) {}

	Ray_Hit()
		: hit_pos(Vector2(0, 0)), ray_len(0.0F), hit(false) {}
};

class Player;

class Raycaster
{
public:
	Raycaster();
	void init(HWND hwnd);
	int on_timer(HWND hwnd);
	void stroke_line(HDC hdc, const Vector2 &p1, const Vector2 &p2, float scale);
	bool inside_map(int cell_x, int cell_y, int cols, int rows);
	Ray_Hit cast_ray(Vector2& p1, Vector2& p2, int cols, int rows);
	void draw_grid(HDC hdc, int cols, int rows, float scale);
	void fill_walls(HDC hdc, int cols, int rows, float scale);
	void redraw_frame(HDC hdc);
	void draw_minimap(HDC hdc, Player& player, float scale);
	void render(HDC hdc, Player& player, float scale);
	void render_game(HDC hdc);

	Player& playerInstance();
	WorldMap& worldmapInstance();

	Vector2 grid_size;
	Player player;
	WorldMap world_map;

	const float r = 5.0F;
	const Vector2 move_speed{0.1F, 0.1F};
	const float rotation_speed = 0.05F;
};
