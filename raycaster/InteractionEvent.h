#pragma once
#include "Event.h"

class InteractionEvent : public Event<Event_Type>
{
public:
	InteractionEvent() 
		: Event<Event_Type>("interaction", Event_Type::INTERACTION) {}
	static void check(Game* game, Node* node, std::string arg1, std::string arg2)
	{
		if (arg1 == "player") {
			int player_pos_x = static_cast<int>(floor(game->world_map.player.pos.x));
			int player_pos_y = static_cast<int>(floor(game->world_map.player.pos.y));

			if (arg2 == "greenwall") {
				size_t green_walls_count = game->worldmapInstance().green_walls.size();
				for (int i = 0; i < green_walls_count; i++) {
					int green_wall_pos_x = static_cast<int>(game->worldmapInstance().green_walls[i]->pos.x);
					int green_wall_pos_y = static_cast<int>(game->worldmapInstance().green_walls[i]->pos.y);
					if (player_pos_x == green_wall_pos_x && player_pos_y == green_wall_pos_y &&
						game->worldmapInstance().green_walls[i]->render) {
						node->active = true;
						return;
					}
				}
			} else if (arg2 == "bluewall") {
				size_t blue_walls_count = game->worldmapInstance().blue_walls.size();
				for (int i = 0; i < blue_walls_count; i++) {
					int blue_wall_pos_x = static_cast<int>(game->worldmapInstance().blue_walls[i]->pos.x);
					int blue_wall_pos_y = static_cast<int>(game->worldmapInstance().blue_walls[i]->pos.y);
					if (player_pos_x == blue_wall_pos_x && player_pos_y == blue_wall_pos_y &&
						game->worldmapInstance().blue_walls[i]->render) {
						node->active = true;
						return;
					}
				}
			} else if (arg2 == "redwall") {
				size_t red_walls_count = game->worldmapInstance().red_walls.size();
				for (int i = 0; i < red_walls_count; i++) {
					int red_wall_pos_x = static_cast<int>(game->worldmapInstance().red_walls[i]->pos.x);
					int red_wall_pos_y = static_cast<int>(game->worldmapInstance().red_walls[i]->pos.y);
					if (player_pos_x == red_wall_pos_x && player_pos_y == red_wall_pos_y &&
						game->worldmapInstance().red_walls[i]->render) {
						node->active = true;
						return;
					}
				}
			} else if (arg2.find("switcher_") == 0) {
				std::string num_str = arg2.substr(9);
				int switcher_id = std::stoi(num_str);
				size_t switchers_count = game->worldmapInstance().switcher.size();
				for (int i = 0; i < switchers_count; i++) {
					int switcher_pos_x = static_cast<int>(game->worldmapInstance().switcher[i].wall->pos.x);
					int switcher_pos_y = static_cast<int>(game->worldmapInstance().switcher[i].wall->pos.y);
					if (player_pos_x == switcher_pos_x && player_pos_y == switcher_pos_y 
						&& game->worldmapInstance().switcher[i].id == switcher_id) {
						node->active = true;
						return;
					}
				}
			}
		}
	}
};