#pragma once
#include <iostream>
#include <vector>
#include "Wall.h"
#include "QuestTimer.h"
#include "Player.h"

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
	
	std::size_t map_height;
	std::size_t map_width;
	
	std::vector<std::vector<Wall>> map;

	std::vector<Wall*> green_walls;
	std::vector<Wall*> blue_walls;
	std::vector<Wall*> red_walls;

	std::vector<Switcher> switcher; 

	QuestTimer quest_timer;
	Player player;

};
