#pragma once
#include "Action.h"


class SetAction : public Action<Action_Type>
{
public:
	SetAction() 
		: Action<Action_Type>("set", Action_Type::SET) {}
	static void act(Game* game, std::string arg1, std::string arg2)
	{
		int num = std::stoi(arg2);
		game->quest_timer.set_timer(num);
	}
};
