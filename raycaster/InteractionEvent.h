#pragma once
#include "Event.h"

class InteractionEvent : public Event<Event_Type>
{
public:
	InteractionEvent() 
		: Event<Event_Type>("interaction", Event_Type::INTERACTION) {}
	static void check(Game& game, std::string arg1, std::string arg2)
	{
		if (game.playerInstance().pos == game.wallInstance().pos) {
			std::cout << "InteractionEvent::check interaction event player pos is equal wall pos" << std::endl;
			game.events_status[Event_Type::INTERACTION] = true;
		} else {
			std::cout << "InteractionEvent::check interaction event player pos doesnt equal wall pos" << std::endl;
			game.events_status[Event_Type::INTERACTION] = false;
		}
	}
};