#pragma once
#include <iostream>
#include <vector>
#include "Wall.h"
#include "QuestTimer.h"
#include "Player.h"

enum class Map_State
{
	START,
	PLAYING,
	VICTORY,
	FAILED,
	RESTART
};

struct Switcher
{
	int id = 0;
	bool active = false;
	Wall* wall = nullptr;
};

class WorldMap
{
private:
public:
	WorldMap();
	void init();
	void ensure_map_initialized();
	void build_wall(COLORREF color, int x, int y);
	void build_switcher(COLORREF color, int id, int x, int y);

	void render_all_walls_with_color(COLORREF color);
	bool map_empty();
	void delete_switcher(int id);

	bool walls_color_count_equals(int num, COLORREF color);

	bool switcher_is_active(int s_id);
	void activate_switcher(int s_id);

	int map_height;
	int map_width;
	int uptime_in_secs;

	std::vector<std::vector<Wall>> map;
	std::vector<Switcher> switcher; 

	Map_State map_state;
	QuestTimer quest_timer;
	Player player;

	Wall* wall_interaction;
};
