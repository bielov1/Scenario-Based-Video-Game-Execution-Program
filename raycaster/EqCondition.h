#pragma once
#include "Condition.h"

class EqCondition : public Condition<Cond_Type>
{
public:
	EqCondition() 
		: Condition<Cond_Type>("eq", Cond_Type::EQ) {}
	static void validate(Game* game, Node* node, std::string arg1, std::string arg2)
	{
		int num = std::stoi(arg2);
		if (game->quest_timer.get_timer_count() == num) {
			node->active = true;
		}
	}
};
