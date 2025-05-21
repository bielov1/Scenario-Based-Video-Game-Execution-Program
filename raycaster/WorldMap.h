#pragma once
#include <iostream>
#include <vector>
#include "Wall.h"

class WorldMap
{
private:
public:
	WorldMap();
	void init(std::size_t h, std::size_t w);
	
	std::size_t map_height;
	std::size_t map_width;
	std::vector<std::vector<Wall>> map;
	std::vector<Wall*> green_walls;
	std::vector<Wall*> blue_walls;
	std::vector<Wall*> red_walls;


};
