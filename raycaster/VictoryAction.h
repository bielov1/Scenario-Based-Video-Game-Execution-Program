#pragma once
#include "Action.h"


class VictoryAction : public Action<Action_Type>
{
public:
	VictoryAction() 
		: Action<Action_Type>("victory", Action_Type::VICTORY) {}
	static void act(Game* game, Node* node)
	{
		if (game->state != Game_State::FAILED) {
			game->state = Game_State::VICTORY;
		}
	}
};
