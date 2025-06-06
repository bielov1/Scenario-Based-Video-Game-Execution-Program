#pragma once
#include "Action.h"

class FailedAction : public Action<Action_Type>
{
public:
	FailedAction() 
		: Action<Action_Type>("failed", Action_Type::FAILED) {}
	static void act(WorldMap* map, Node* node)
	{
		if (map->map_state != Map_State::VICTORY) {
			map->map_state = Map_State::FAILED;
		}
	}
};
