#include <Windows.h>
#include "WorldMap.h"


WorldMap::WorldMap()
	: map_height(0), map_width(0), uptime_in_secs(0), quest_timer(), player(), map_state(Map_State::START)
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

void WorldMap::build_wall(Wall_Color color, int x, int y)
{
	ensure_map_initialized();

	if (x >= map_width || y >= map_height) {
		std::cerr << "build_wall: out of bounds!\n";
		return;
	}

	switch(color) {
	case Wall_Color::RED:
		map[y][x].rgb_color = RGB(0, 0, 255);
		red_walls.push_back(&map[y][x]);
		built_walls.push_back(&map[y][x]);
		break;
	case Wall_Color::GREEN:
		map[y][x].rgb_color = RGB(0, 255, 0);
		green_walls.push_back(&map[y][x]);
		built_walls.push_back(&map[y][x]);
		break;
	case Wall_Color::BLUE:
		map[y][x].rgb_color = RGB(255, 0, 0);
		blue_walls.push_back(&map[y][x]);
		built_walls.push_back(&map[y][x]);
		break;
	default:
		return;
	}
}

void WorldMap::build_switcher(Wall_Color color, int id, int x, int y)
{
	ensure_map_initialized();

	if (x >= map_width || y >= map_height) {
		std::cerr << "build_wall: out of bounds!\n";
		return;
	}

	Switcher sw;
	switch(color) {
	case Wall_Color::RED:
		map[y][x].rgb_color = RGB(0, 0, 255);
		map[y][x].render = true;
		sw.id = id;
		sw.wall = &map[y][x];
		switcher.push_back(sw);
		break;
	case Wall_Color::GREEN:
		map[y][x].rgb_color = RGB(0, 255, 0);
		map[y][x].render = true;
		sw.id = id;
		sw.wall = &map[y][x];
		switcher.push_back(sw);
		break;
	case Wall_Color::BLUE:
		map[y][x].rgb_color = RGB(255, 0, 0);
		map[y][x].render = true;
		sw.id = id;
		sw.wall = &map[y][x];
		switcher.push_back(sw);
		break;
	default:
		return;
	}
}

void WorldMap::render_all_blue_walls()
{
	size_t blue_walls_count = blue_walls.size();
	for (int i = 0; i < blue_walls_count; i++) {
		blue_walls[i]->render = true;
	}
}

void WorldMap::render_all_green_walls()
{
	size_t green_walls_count = green_walls.size();
	for (int i = 0; i < green_walls_count; i++) {
		green_walls[i]->render = true;
	}
}

void WorldMap::render_all_red_walls()
{
	size_t red_walls_count = red_walls.size();
	for (int i = 0; i < red_walls_count; i++) {
		red_walls[i]->render = true;
	}
}

void WorldMap::delete_switcher(int id)
{
	for (auto it = switcher.begin(); it != switcher.end(); ++it) {
		if (it->id == id) {
			it->wall->render = false;
			//switcher.erase(it);
			break;
		}
	}
}

bool WorldMap::greenwalls_count_equals(int num) 
{
	int green_walls_count = 0;

	for (Wall* w : green_walls) {
		if (w->render) {
			green_walls_count++;
		}
	}

	return num == green_walls_count;
}

bool WorldMap::bluewalls_count_equals(int num) 
{
	int blue_walls_count = 0;

	for (Wall* w : blue_walls) {
		if (w->render) {
			blue_walls_count++;
		}
	}

	return num == blue_walls_count;
}

bool WorldMap::redwalls_count_equals(int num) 
{
	int red_walls_count = 0;

	for (Wall* w : red_walls) {
		if (w->render) {
			red_walls_count++;
		}
	}

	return num == red_walls_count;
}

bool WorldMap::greenwalls_count_not_equals(int num) 
{
	int green_walls_count = 0;

	for (Wall* w : green_walls) {
		if (w->render) {
			green_walls_count++;
		}
	}

	return num != green_walls_count;
}

bool WorldMap::bluewalls_count_not_equals(int num) 
{
	int blue_walls_count = 0;

	for (Wall* w : blue_walls) {
		if (w->render) {
			blue_walls_count++;
		}
	}

	return num != blue_walls_count;
}

bool WorldMap::redwalls_count_not_equals(int num) 
{
	int red_walls_count = 0;

	for (Wall* w : red_walls) {
		if (w->render) {
			red_walls_count++;
		}
	}

	return num != red_walls_count;
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


void WorldMap::clear_walls()
{
	red_walls.clear();
	green_walls.clear();
	blue_walls.clear();
}