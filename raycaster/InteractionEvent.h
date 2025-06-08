#pragma once
#include "Event.h"

class InteractionEvent : public Event<Event_Type>
{
public:
	InteractionEvent() 
		: Event<Event_Type>("interaction", Event_Type::INTERACTION) {}
	static void check(WorldMap* map, Node* node, std::string arg1, std::string arg2)
	{
		if (arg1 == "player") {
			int player_pos_x = static_cast<int>(floor(map->player.pos.x));
			int player_pos_y = static_cast<int>(floor(map->player.pos.y));

			if (arg2 == "wall") {
				size_t build_walls_count = map->built_walls.size();
				for (int i = 0; i < build_walls_count; i++) {
					int wall_pos_x = static_cast<int>(map->built_walls[i]->pos.x);
					int wall_pos_y = static_cast<int>(map->built_walls[i]->pos.y);
					if (player_pos_x == wall_pos_x && player_pos_y == wall_pos_y &&
						map->built_walls[i]->render) {
						node->active = true;
						return;
					}
				}
			} else if (arg2.find("switcher_") == 0) {
				std::string num_str = arg2.substr(9);
				int switcher_id = std::stoi(num_str);
				size_t switchers_count = map->switcher.size();
				for (int i = 0; i < switchers_count; i++) {
					int switcher_pos_x = static_cast<int>(map->switcher[i].wall->pos.x);
					int switcher_pos_y = static_cast<int>(map->switcher[i].wall->pos.y);
					if (player_pos_x == switcher_pos_x && player_pos_y == switcher_pos_y 
						&& map->switcher[i].id == switcher_id) {
						node->active = true;
						return;
					}
				}
			}
		}
	}
};