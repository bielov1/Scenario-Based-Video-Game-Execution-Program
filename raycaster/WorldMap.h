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

enum class Wall_Color
{
	RED,
	GREEN,
	BLUE
};

struct Switcher
{
	int id;
	Wall* wall;
};

class WorldMap
{
private:
public:
	WorldMap();
	void init();
	void ensure_map_initialized();
	void build_wall(Wall_Color color, int x, int y);
	void build_switcher(Wall_Color color, int id, int x, int y);
	void render_all_blue_walls();
	void render_all_green_walls();
	void render_all_red_walls();
	void delete_switcher(int id);
	bool all_blue_walls_are_destroyed();
	bool all_green_walls_are_destroyed();
	bool all_red_walls_are_destroyed();
	void clear_walls();

	int map_height;
	int map_width;
	int uptime_in_secs;

	std::vector<std::vector<Wall>> map;

	std::vector<Wall*> green_walls;
	std::vector<Wall*> blue_walls;
	std::vector<Wall*> red_walls;

	std::vector<Switcher> switcher; 

	Map_State map_state;
	QuestTimer quest_timer;
	Player player;
};
