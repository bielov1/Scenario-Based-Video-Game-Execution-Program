#pragma once
#include "Action.h"


class SubAction : public Action<Action_Type>
{
public:
	SubAction() 
		: Action<Action_Type>("sub", Action_Type::SUB) {}
	static void act(Game* game, std::string arg1, std::string arg2)
	{
		int num = std::stoi(arg2);
		game->quest_timer.reduce_time(num);
	}
};
