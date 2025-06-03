#include <Windows.h>
#include "WorldMap.h"

WorldMap::WorldMap()
	: map_height(0), map_width(0), quest_timer() {}

void WorldMap::init()
{
	map.clear();
	for (float i = 0; i < map_height; i++) {
		std::vector<Wall> row;
		for (float j = 0; j < map_width; j++) {
			row.push_back(Wall(Vector2(j, i), RGB(0, 0, 0), false));
		}
		map.push_back(row);
	}
}
void WorldMap::ensure_map_initialized()
{
	if (map.empty() && map_width > 0 && map_height > 0) {
		init();
	}
}

void WorldMap::build_wall(Wall_Color color, int x, int y)
{
	ensure_map_initialized();

	if (x >= map_width || y >= map_height) {
		std::cerr << "build_wall: out of bounds!\n";
		return;
	}

	switch(color) {
	case Wall_Color::RED:
		map[x][y].rgb_color = RGB(0, 0, 255);
		map[x][y].render = true;
		red_walls.push_back(&map[x][y]);
		break;
	case Wall_Color::GREEN:
		map[x][y].rgb_color = RGB(0, 255, 0);
		map[x][y].render = true;
		green_walls.push_back(&map[x][y]);
		break;
	case Wall_Color::BLUE:
		map[x][y].rgb_color = RGB(255, 0, 0);
		map[x][y].render = true;
		blue_walls.push_back(&map[x][y]);
		break;
	default:
		return;
	}
}

void WorldMap::clear_walls()
{
	red_walls.clear();
	green_walls.clear();
	blue_walls.clear();
}