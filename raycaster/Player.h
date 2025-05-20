#pragma once
#include <string>

class Player
{
public:
	int hp = 0;
	int pos = 0;
	std::string moving = "STAND";

	void init(int hp, int pos, std::string mov)
	{
		this->hp = hp;
		this->pos = pos;
		this->moving = mov;
	}
};
