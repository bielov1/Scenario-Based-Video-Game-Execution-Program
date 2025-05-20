#pragma once
#include "Condition.h"

class AnyCondition : public Condition<Cond_Type>
{
public:
	AnyCondition() 
		: Condition<Cond_Type>("any", Cond_Type::ANY) {}
	static void validate(Game& game, Event_Type e_type)
	{
		std::cout << "AnyCondition::validate condition any is returns true." << std::endl;
		game.conditions_status_for_event_type[e_type].push_back(true);
	}
};
