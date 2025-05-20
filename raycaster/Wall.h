#pragma once
#include <string>

class Wall
{
public:
	int pos = 5;
	bool render = true;
	void init(int pos)
	{
		this->pos = pos;
	}
};
