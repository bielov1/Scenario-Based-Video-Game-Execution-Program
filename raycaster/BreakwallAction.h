#pragma once
#include "Action.h"


class BreakwallAction : public Action<Action_Type>
{
public:
	BreakwallAction() 
		: Action<Action_Type>("breakwall", Action_Type::BREAKWALL) {}

	static void act(WorldMap* map, Node* node)
	{
		Wall* wall = map->wall_interaction;
		wall->render = false;
	}
};
