#pragma once
#include "Event.h"

class ObjectEvent : public Event<Event_Type>
{
public:
	ObjectEvent() 
		: Event<Event_Type>("object", Event_Type::OBJECT) {}
	static void check(Game* game, Node* node, std::string arg1)
	{
		if (arg1 == "wall") {
			if (arg2 == "blue") {
			
			} else if (arg2 == "green") {
			
			} else if (arg2 == "red") {
			
			}
		}
	}
};