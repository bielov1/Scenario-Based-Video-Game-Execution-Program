#pragma once
#include "Action.h"


class BuildAction : public Action<Action_Type>
{
public:
	BuildAction() 
		: Action<Action_Type>("build", Action_Type::BUILD) {}
	static const Node* get_root(const Node* node)
	{
		while (node && node->prev_node) {
			node = node->prev_node;
		}
		return node;
	}

	static void act(Game* game, Node* node, 
					std::string arg1, std::string arg2, std::string arg3, std::string arg4)
	{
		const Node* root = get_root(node);
		int num1 = std::stoi(arg3);
		int num2 = std::stoi(arg4);
		Event_Type t = GetEventTypeByID(root->id);
		if (t == Event_Type::MAP) {
			if (arg1 == "wall") {
				if (arg2 == "blue") {
					game->world_map.build_wall(Wall_Color::BLUE, num1, num2);
				} else if (arg2 == "green") {
					game->world_map.build_wall(Wall_Color::GREEN, num1, num2);
				} else if (arg2 == "red") {
					game->world_map.build_wall(Wall_Color::RED, num1, num2);
				}
			}
		}
	}
};
