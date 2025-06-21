#pragma once
#include "Action.h"


class BuildAction : public Action<Action_Type>
{
public:
	BuildAction() 
		: Action<Action_Type>("build", Action_Type::BUILD) {}

	static void act(WorldMap* map, Node* node, 
					std::string arg1, std::string arg2, std::string arg3, std::string arg4)
	{
		int num1 = std::stoi(arg3);
		int num2 = std::stoi(arg4);
		
		if (arg1 == "wall") {
			if (arg2 == "blue") {
				map->build_wall(RGB(255, 0, 0), num1, num2);
			} else if (arg2 == "green") {
				map->build_wall(RGB(0, 255, 0), num1, num2);
			} else if (arg2 == "red") {
				map->build_wall(RGB(0, 0, 255), num1, num2);
			}
		} else if (arg1.find("switcher_") == 0) {
			std::string num_str = arg1.substr(9);
			int switcher_id = std::stoi(num_str);
			if (arg2 == "blue") {
				map->build_switcher(RGB(255, 0, 0), switcher_id, num1, num2);
			} else if (arg2 == "green"){
				map->build_switcher(RGB(0, 255, 0), switcher_id, num1, num2);
			} else if (arg2 == "red") {
				map->build_switcher(RGB(0, 0, 255), switcher_id, num1, num2);
			}
		}
		
	}
};
