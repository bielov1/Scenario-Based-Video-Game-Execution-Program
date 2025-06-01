#pragma once
#include "Condition.h"

class NoneCondition : public Condition<Cond_Type>
{
public:
	NoneCondition() 
		: Condition<Cond_Type>("none", Cond_Type::NONE) {}
	static void validate(Game* game, Node* node)
	{
		node->active = false;
	}
};
