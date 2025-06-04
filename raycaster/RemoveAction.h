#pragma once
#include "Action.h"


class RemoveAction : public Action<Action_Type>
{
public:
	RemoveAction() 
		: Action<Action_Type>("remove", Action_Type::REMOVE) {}
	static const Node* get_root(const Node* node)
	{
		while (node && node->prev_node) {
			node = node->prev_node;
		}
		return node;
	}

	static void act(Game* game, Node* node, std::string arg1)
	{
		if (arg1.find("switcher_") == 0) {
			std::string num_str = arg1.substr(9);
			int switcher_id = std::stoi(num_str);
			game->worldmapInstance().delete_switcher(switcher_id);
		}
	}
};
