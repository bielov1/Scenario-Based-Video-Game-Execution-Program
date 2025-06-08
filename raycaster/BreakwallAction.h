#pragma once
#include "Action.h"


class BreakwallAction : public Action<Action_Type>
{
public:
	BreakwallAction() 
		: Action<Action_Type>("breakwall", Action_Type::BREAKWALL) {}

	static const Node* get_root(const Node* node)
	{
		while (node && node->prev_node) {
			node = node->prev_node;
		}
		return node;
	}

	static void act(WorldMap* map, Node* node, std::string arg)
	{
		const Node* root = get_root(node);
		std::string root_arg1 = GetEventFirstArgById(root->id);

		if (root_arg1 == "player") {
			int player_pos_x = static_cast<int>(floor(map->player.pos.x));
			int player_pos_y = static_cast<int>(floor(map->player.pos.y));
			if (arg == "greenwall") {
				size_t green_walls_count = map->green_walls.size();
				for (int i = 0; i < green_walls_count; i++) {
					int green_wall_pos_x = static_cast<int>(map->green_walls[i]->pos.x);
					int green_wall_pos_y = static_cast<int>(map->green_walls[i]->pos.y);
					if (player_pos_x == green_wall_pos_x && player_pos_y == green_wall_pos_y) {
						map->green_walls[i]->render = false;
						return;
					}
				}
			} else if (arg == "bluewall") {
				size_t blue_walls_count = map->blue_walls.size();
				for (int i = 0; i < blue_walls_count; i++) {
					int blue_wall_pos_x = static_cast<int>(map->blue_walls[i]->pos.x);
					int blue_wall_pos_y = static_cast<int>(map->blue_walls[i]->pos.y);
					if (player_pos_x == blue_wall_pos_x && player_pos_y == blue_wall_pos_y) {
						map->blue_walls[i]->render = false;
						return;
					}
				}
			} else if (arg == "redwall") {
				size_t red_walls_count = map->red_walls.size();
				for (int i = 0; i < red_walls_count; i++) {
					int red_wall_pos_x = static_cast<int>(map->red_walls[i]->pos.x);
					int red_wall_pos_y = static_cast<int>(map->red_walls[i]->pos.y);
					if (player_pos_x == red_wall_pos_x && player_pos_y == red_wall_pos_y) {
						map->red_walls[i]->render = false;
						return;
					}
				}
			}
		}
	}
};
