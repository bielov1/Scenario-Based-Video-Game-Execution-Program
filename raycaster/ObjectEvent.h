#pragma once
#include "Event.h"
class ObjectEvent : public Event<Event_Type>
{
private:
public:
	ObjectEvent() 
		: Event<Event_Type>("object", Event_Type::OBJECT) {}
	static void check(Game* game, Node* node, std::string arg)
	{
		std::cout << "checking ObjectEvent::check with argument: "
			<< arg
			<< std::endl;
	}
};
