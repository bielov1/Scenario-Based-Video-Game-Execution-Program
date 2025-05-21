#include "WorldMap.h"

WorldMap::WorldMap()
	: map_height(0), map_width(0) {}

void WorldMap::init(std::size_t h, std::size_t w)
{
	//h, std::vector<Wall>(w)
	map_height = h;
	map_width = w;

	map.clear();

	for (float i = 0; i < map_height; i++) {
		std::vector<Wall> row;
		for (float j = 0; j < map_width; j++) {
			row.push_back(Wall(Vector2(j, i), nullptr, false));
		}
		map.push_back(row);
	}

	for (int i = 0; i < map_height; i++) {
		map[i][4].wall_color = AsConfig::blue_brush;
		map[i][4].render = true;
		blue_walls.push_back(&map[i][4]);
	}

	for (int i = 0; i < map_height; i++) {
		map[i][6].wall_color = AsConfig::blue_brush;
		map[i][6].render = true;
		blue_walls.push_back(&map[i][6]);
	}

	map[0][5].wall_color = AsConfig::green_brush;
	map[0][5].render = true;
	green_walls.push_back(&map[0][5]);
	map[2][5].wall_color = AsConfig::green_brush;
	map[2][5].render = true;
	green_walls.push_back(&map[2][5]);
	map[5][5].wall_color = AsConfig::green_brush;
	map[5][5].render = true;
	green_walls.push_back(&map[5][5]);
	map[7][5].wall_color = AsConfig::green_brush;
	map[7][5].render = true;
	green_walls.push_back(&map[7][5]);
	map[0][0].wall_color = AsConfig::green_brush;
	map[0][0].render = true;
	green_walls.push_back(&map[0][0]);
	map[9][9].wall_color = AsConfig::green_brush;
	map[9][9].render = true;
	green_walls.push_back(&map[9][9]);
}