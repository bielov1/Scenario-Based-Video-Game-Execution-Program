#pragma once
#include "Action.h"


class ShowallAction : public Action<Action_Type>
{
public:
	ShowallAction() 
		: Action<Action_Type>("showall", Action_Type::SHOWALL) {}
	static const Node* get_root(const Node* node)
	{
		while (node && node->prev_node) {
			node = node->prev_node;
		}
		return node;
	}

	static void act(WorldMap* map, Node* node, std::string arg1, std::string arg2)
	{
		const std::string prefix = "wall.";
		if (arg1.rfind(prefix, 0) == 0) {
			std::string after_dot = arg1.substr(prefix.length());
			if (after_dot == "color") {
				if (arg2 == "blue") {
					map->render_all_blue_walls();
				} else if (arg2 == "green") {
					map->render_all_green_walls();
				} else if (arg2 == "red") {
					map->render_all_red_walls();
				}
			}
		} 
	}
};
