#pragma once
#include "Event.h"

class MapEvent : public Event<Event_Type>
{
public:
	MapEvent() 
		: Event<Event_Type>("map", Event_Type::MAP) {}
	static void check(Game* game, Node* node)
	{
		if (!node->blocked) {
			node->active = true;
			node->blocked = true;
		}
	}
};