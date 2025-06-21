#include <Windows.h>
#include "WorldMap.h"


WorldMap::WorldMap()
	: map_height(0), map_width(0), uptime_in_secs(0), quest_timer(), player(), wall_interaction(nullptr), map_state(Map_State::START)
{}

void WorldMap::init()
{
	map_state = Map_State::PLAYING;
	map.clear();
	for (double i = 0; i < map_height; i++) {
		std::vector<Wall> row;
		for (double j = 0; j < map_width; j++) {
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

void WorldMap::build_wall(COLORREF color, int x, int y)
{
	ensure_map_initialized();

	if (x >= map_width || y >= map_height) {
		std::cerr << "build_wall: out of bounds!\n";
		return;
	}

	map[y][x].rgb_color = color;
}

void WorldMap::build_switcher(COLORREF color, int id, int x, int y)
{
	ensure_map_initialized();

	if (x >= map_width || y >= map_height) {
		std::cerr << "build_wall: out of bounds!\n";
		return;
	}

	Switcher sw;
	map[y][x].rgb_color = color;
	map[y][x].render = true;
	sw.id = id;
	sw.wall = &map[y][x];
	switcher.push_back(sw);
}

void WorldMap::render_all_walls_with_color(COLORREF color)
{
	size_t map_size = map.size();
	for (int i = 0; i < map_size; i++) {
		for (int j = 0; j < map_size; j++) {
			if (!map[i][j].render && map[i][j].rgb_color == color) {
				map[i][j].render = true;
			}
		}
	}
}

bool WorldMap::map_empty() {
	for (const auto& row : map) {
		for (const Wall& wall : row) {
			if (wall.render) {
				return false;
			}
		}
	}

	return true;
}

void WorldMap::delete_switcher(int id)
{
	for (auto it = switcher.begin(); it != switcher.end(); ++it) {
		if (it->id == id) {
			it->wall->render = false;
			break;
		}
	}
}

bool WorldMap::walls_color_count_equals(int num, COLORREF color) 
{
	int walls_color_count = 0;

	for (const auto& row : map) {
		for (const Wall& wall : row) {
			if (wall.render && wall.rgb_color == color) {
				walls_color_count++;
			}
		}
	}

	return num == walls_color_count;
}

void WorldMap::activate_switcher(int s_id)
{
	for (Switcher& s : switcher) {
		if (s.id == s_id) {
			s.active = true;
		}
	}
}

bool WorldMap::switcher_is_active(int s_id)
{
	for (const Switcher s : switcher) {
		if (s.id == s_id && s.active) {
			return true;
		}
	}

	return false;
}