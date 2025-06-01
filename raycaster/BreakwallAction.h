#pragma once
#include "Action.h"


class BreakwallAction : public Action<Action_Type>
{
public:
	BreakwallAction() 
		: Action<Action_Type>("breakwall", Action_Type::BREAKWALL) {}
	static void act(Game* game, std::string arg)
	{
		int player_pos_x = static_cast<int>(floor(game->raycaster.player.pos.x));
		int player_pos_y = static_cast<int>(floor(game->raycaster.player.pos.y));
		if (arg == "greenwall") {
			size_t green_walls_count = game->raycaster.worldmapInstance().green_walls.size();
			for (int i = 0; i < green_walls_count; i++) {
				int green_wall_pos_x = static_cast<int>(game->raycaster.worldmapInstance().green_walls[i]->pos.x);
				int green_wall_pos_y = static_cast<int>(game->raycaster.worldmapInstance().green_walls[i]->pos.y);
				if (player_pos_x == green_wall_pos_x && player_pos_y == green_wall_pos_y) {
					game->raycaster.worldmapInstance().green_walls[i]->render = false;
					return;
				}
			}
		} else if (arg == "bluewall") {
			size_t blue_walls_count = game->raycaster.worldmapInstance().blue_walls.size();
			for (int i = 0; i < blue_walls_count; i++) {
				int blue_wall_pos_x = static_cast<int>(game->raycaster.worldmapInstance().blue_walls[i]->pos.x);
				int blue_wall_pos_y = static_cast<int>(game->raycaster.worldmapInstance().blue_walls[i]->pos.y);
				if (player_pos_x == blue_wall_pos_x && player_pos_y == blue_wall_pos_y) {
					game->raycaster.worldmapInstance().blue_walls[i]->render = false;
					return;
				}
			}
		}
	}
};
