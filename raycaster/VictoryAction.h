#pragma once
#include "Action.h"


class VictoryAction : public Action<Action_Type>
{
public:
	VictoryAction() 
		: Action<Action_Type>("victory", Action_Type::VICTORY) {}
	static void act(WorldMap* map, Node* node)
	{
		if (map->map_state != Map_State::FAILED) {
			map->map_state = Map_State::VICTORY;
		}
	}
};
