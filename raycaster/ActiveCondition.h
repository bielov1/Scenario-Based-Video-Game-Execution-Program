#pragma once
#include "Condition.h"

class ActiveCondition : public Condition<Cond_Type>
{
public:
	ActiveCondition() 
		: Condition<Cond_Type>("active", Cond_Type::ACTIVE) {}
	static void validate(WorldMap* map, Node* node, std::string arg1)
	{
		if (arg1.find("switcher_") == 0) {
			std::string num_str = arg1.substr(9);
			int switcher_id = std::stoi(num_str);
			if (map->switcher_is_active(switcher_id)) {
				node->active = true;
			}
		}
	}
};
