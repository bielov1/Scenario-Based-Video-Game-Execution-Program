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
	
	static void validate(Game* game, Node* node, std::string arg1, std::string arg2)
	{
		const Node* root = get_root(node);
		int num = std::stoi(arg2);
		const std::string prefix = "here.";
		if (arg1.rfind(prefix, 0) == 0) {
			std::string after_dot = arg1.substr(prefix.length());
			Event_Type t = GetEventTypeByID(root->id);
			if (t == Event_Type::TIMER) {
				if (after_dot == "time") {
					if (game->world_map.quest_timer.get_timer_count() == num) {
						node->active = true;
					}
				} 
			}
		} 

	}
};
