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

class WorldMap
{
private:
public:
	WorldMap();
	void init();
	void ensure_map_initialized();
	void build_wall(Wall_Color color, int x, int y);
	void clear_walls();
	
	std::size_t map_height;
	std::size_t map_width;
	std::vector<std::vector<Wall>> map;
	std::vector<Wall*> green_walls;
	std::vector<Wall*> blue_walls;
	std::vector<Wall*> red_walls;

	QuestTimer quest_timer;
	//Player player;

};
