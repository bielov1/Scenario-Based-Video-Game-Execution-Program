#pragma once
#include "Event.h"

class InteractionEvent : public Event<Event_Type>
{
public:
	InteractionEvent() 
		: Event<Event_Type>("interaction", Event_Type::INTERACTION) {}

	static void check(WorldMap* map, Node* node, std::string arg1, std::string arg2)
	{
		std::string first_entity, second_entity;

		if (arg1 == "player" || arg2 == "player") {
			if (arg1 == "player") {
				first_entity = arg1;
				second_entity = arg2;
			} else {
				first_entity = arg2;
				second_entity = arg1;
			}

			checkPlayerInteraction(map, node, second_entity);
		}
	}

private:
	static void checkPlayerInteraction(WorldMap* map, Node* node, const std::string& other_entity)
	{
		int player_pos_x = static_cast<int>(floor(map->player.pos.x));
		int player_pos_y = static_cast<int>(floor(map->player.pos.y));

		if (other_entity == "wall") {
			checkWallCollision(map, node, player_pos_x, player_pos_y);
		} else if (other_entity.find("switcher_") == 0) {
			checkSwitcherCollision(map, node, player_pos_x, player_pos_y, other_entity);
		}
	}

	static void checkWallCollision(WorldMap* map, Node* node, int player_x, int player_y)
	{
		size_t map_size = map->map.size();
		for (size_t i = 0; i < map_size; i++) {
			for (size_t j = 0; j < map->map[i].size(); j++) {
				int wall_pos_x = static_cast<int>(map->map[i][j].pos.x);
				int wall_pos_y = static_cast<int>(map->map[i][j].pos.y);
				if (player_x == wall_pos_x && player_y == wall_pos_y &&
					map->map[i][j].render) {
					map->wall_interaction = &map->map[i][j];
					node->active = true;
					return;
				}
			}
		}
	}

	static void checkSwitcherCollision(WorldMap* map, Node* node, int player_x, int player_y, 
		const std::string& switcher_name)
	{
		std::string num_str = switcher_name.substr(9);
		int switcher_id = std::stoi(num_str);
		size_t switchers_count = map->switcher.size();

		for (size_t i = 0; i < switchers_count; i++) {
			int switcher_pos_x = static_cast<int>(map->switcher[i].wall->pos.x);
			int switcher_pos_y = static_cast<int>(map->switcher[i].wall->pos.y);
			if (player_x == switcher_pos_x && player_y == switcher_pos_y 
				&& map->switcher[i].id == switcher_id && map->switcher[i].wall->render) {
				map->wall_interaction = map->switcher[i].wall;
				node->active = true;
				return;
			}
		}
	}
};