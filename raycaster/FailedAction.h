#pragma once
#include "Action.h"


class FailedAction : public Action<Action_Type>
{
public:
	FailedAction() 
		: Action<Action_Type>("failed", Action_Type::FAILED) {}
	static void act(Game* game, Node* node)
	{
		if (game->state != Game_State::VICTORY) {
			game->state = Game_State::FAILED;
		}
	}
};
