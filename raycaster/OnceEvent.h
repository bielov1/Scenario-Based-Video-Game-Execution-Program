#pragma once
#include "Event.h"

class OnceEvent : public Event<Event_Type>
{
public:
	OnceEvent() 
		: Event<Event_Type>("once", Event_Type::ONCE) {}
	static void check(Game* game, Node* node)
	{
		if (!node->blocked) {
			node->active = true;
			node->blocked = true;
		}
	}
};