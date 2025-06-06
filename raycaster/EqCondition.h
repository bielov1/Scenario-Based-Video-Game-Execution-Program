#pragma once
#include "Condition.h"

class EqCondition : public Condition<Cond_Type>
{
public:
	EqCondition() 
		: Condition<Cond_Type>("eq", Cond_Type::EQ) {}
	static const Node* get_root(const Node* node)
	{
		while (node && node->prev_node) {
			node = node->prev_node;
		}
		return node;
	}
	
	static void validate(WorldMap* map, Node* node, std::string arg1, std::string arg2)
	{
		const Node* root = get_root(node);
		int num = std::stoi(arg2);
		const std::string prefix_here = "here.";
		const std::string prefix_this = "this2.";
		if (arg1.rfind(prefix_here, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_here.length());
			Event_Type t = GetEventTypeByID(root->id);
			if (t == Event_Type::TIMER) {
				if (after_dot == "time") {
					if (map->quest_timer.get_timer_count() == num) {
						node->active = true;
					}
				} 
			}
		} else if (arg1.rfind(prefix_this, 0) == 0) {
			std::string root_arg2 = GetEventSecondArgById(root->id);
			if (root_arg2.find("bluewall") == 0) {
				if (map->all_blue_walls_are_destroyed()) {
					node->active = true;
				}
			} else if (root_arg2.find("greenwall") == 0) {
				if (map->all_green_walls_are_destroyed()) {
					node->active = true;
				}
			} else if (root_arg2.find("redwall") == 0) {
				if (map->all_red_walls_are_destroyed()) {
					node->active = true;
				}
			}
		}

	}
};
