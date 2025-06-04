#pragma once
#include "Action.h"


class BuildAction : public Action<Action_Type>
{
public:
	BuildAction() 
		: Action<Action_Type>("build", Action_Type::BUILD) {}

	static void act(Game* game, Node* node, 
					std::string arg1, std::string arg2, std::string arg3, std::string arg4)
	{
		int num1 = std::stoi(arg3);
		int num2 = std::stoi(arg4);
		
		if (arg1 == "wall") {
			if (arg2 == "blue") {
				game->world_map.build_wall(Wall_Color::BLUE, num1, num2);
			} else if (arg2 == "green") {
				game->world_map.build_wall(Wall_Color::GREEN, num1, num2);
			} else if (arg2 == "red") {
				game->world_map.build_wall(Wall_Color::RED, num1, num2);
			}
		} else if (arg1.find("switcher_") == 0) {
			std::string num_str = arg1.substr(9);
			int switcher_id = std::stoi(num_str);
			if (arg2 == "blue") {
				game->world_map.build_switcher(Wall_Color::BLUE, switcher_id, num1, num2);
			} else if (arg2 == "green"){
				game->world_map.build_switcher(Wall_Color::GREEN, switcher_id, num1, num2);
			} else if (arg2 == "red") {
				game->world_map.build_switcher(Wall_Color::RED, switcher_id, num1, num2);
			}
		}
		
	}
};
