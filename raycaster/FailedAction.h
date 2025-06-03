#pragma once
#include "Action.h"


class FailedAction : public Action<Action_Type>
{
public:
	FailedAction() 
		: Action<Action_Type>("failed", Action_Type::FAILED) {}
	static void act(Game* game)
	{
		game->state = Game_State::FAILED;
	}
};
