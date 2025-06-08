#pragma once
#include "Action.h"


class SetAction : public Action<Action_Type>
{
public:
	SetAction() 
		: Action<Action_Type>("set", Action_Type::SET) {}
	static const Node* get_root(const Node* node)
	{
		while (node && node->prev_node) {
			node = node->prev_node;
		}
		return node;
	}

	static void act(WorldMap* map, Node* node, std::string arg1, std::string arg2)
	{
		const Node* root = get_root(node);
		int num = std::stoi(arg2);
		const std::string prefix_here = "here.";
		const std::string timer_prefix = "timer.";
		const std::string prefix_this2 = "this2.";
		if (arg1.rfind(prefix_here, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_here.length());
			Event_Type t = GetEventTypeByID(root->id);
			if (t == Event_Type::MAP) {
				if (after_dot == "time") {
					map->quest_timer.set_timer(num);
				} else if (after_dot == "width") {
					map->map_width = num;
				} else if (after_dot == "height") {
					map->map_height = num;
				} else if (after_dot == "posx") {
					map->player.set_posx(num);
				} else if (after_dot == "posy") {
					map->player.set_posy(num);
				}
			}
		} else if (arg1.rfind(timer_prefix, 0) == 0) {
			std::string after_dot = arg1.substr(timer_prefix.length());
			if (after_dot == "start") {
				map->quest_timer.activate();
			}
		} else if (arg1.rfind(prefix_this2, 0) == 0) {
			std::string root_arg2 = GetEventSecondArgById(root->id);
			if (root_arg2.find("switcher_") == 0) {
				if (num > 0) {
					std::string num_str = root_arg2.substr(9);
					int switcher_id = std::stoi(num_str);
					map->activate_switcher(switcher_id);
				}
			}
		}
	}
};
