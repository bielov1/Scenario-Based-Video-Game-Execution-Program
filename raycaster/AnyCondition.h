#pragma once
#include "Condition.h"

class AnyCondition : public Condition<Cond_Type>
{
public:
	AnyCondition() 
		: Condition<Cond_Type>("any", Cond_Type::ANY) {}
	static void validate(WorldMap* map, Node* node)
	{
		node->active = true;
	}
};
