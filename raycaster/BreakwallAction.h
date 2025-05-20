#pragma once
#include "Action.h"


class BreakwallAction : public Action<Action_Type>
{
public:
	BreakwallAction() 
		: Action<Action_Type>("breakwall", Action_Type::BREAKWALL) {}
	static void act(Game& game)
	{
		std::cout << "BreakwallAction::act action breakwall disabled wall render." << std::endl;
		game.wallInstance().render = false;
	}
};
