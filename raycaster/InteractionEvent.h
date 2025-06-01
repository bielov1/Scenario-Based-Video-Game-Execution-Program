#pragma once
#include "Event.h"

class InteractionEvent : public Event<Event_Type>
{
public:
	InteractionEvent() 
		: Event<Event_Type>("interaction", Event_Type::INTERACTION) {}
	static void check(Game* game, Node* node, std::string arg1, std::string arg2)
	{
		int player_pos_x = static_cast<int>(floor(game->raycaster.player.pos.x));
		int player_pos_y = static_cast<int>(floor(game->raycaster.player.pos.y));

		if (arg2 == "greenwall") {
			size_t green_walls_count = game->raycaster.worldmapInstance().green_walls.size();
			for (int i = 0; i < green_walls_count; i++) {
				int green_wall_pos_x = static_cast<int>(game->raycaster.worldmapInstance().green_walls[i]->pos.x);
				int green_wall_pos_y = static_cast<int>(game->raycaster.worldmapInstance().green_walls[i]->pos.y);
				if (player_pos_x == green_wall_pos_x && player_pos_y == green_wall_pos_y) {
					node->active = true;
					return;
				} else {
					node->active = false;
				}
			}
		} else if (arg2 == "bluewall") {
			size_t blue_walls_count = game->raycaster.worldmapInstance().blue_walls.size();
			for (int i = 0; i < blue_walls_count; i++) {
				int blue_wall_pos_x = static_cast<int>(game->raycaster.worldmapInstance().blue_walls[i]->pos.x);
				int blue_wall_pos_y = static_cast<int>(game->raycaster.worldmapInstance().blue_walls[i]->pos.y);
				if (player_pos_x == blue_wall_pos_x && player_pos_y == blue_wall_pos_y) {
					node->active = true;
					return;
				} else {
					node->active = false;
				}
			}
		}
	}
};