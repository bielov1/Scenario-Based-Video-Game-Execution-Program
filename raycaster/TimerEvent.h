#pragma once
#include "Event.h"

class TimerEvent : public Event<Event_Type>
{
public:
	TimerEvent() 
		: Event<Event_Type>("timer", Event_Type::TIMER) {}
	static void check(Game* game, Node* node, std::string arg)
	{
		static std::map<Node*, int> last_trigger;
		int interval = std::stoi(arg);
		if (interval <= 0) return;

		if (game->uptime_in_secs % interval == 0 &&
			last_trigger[node] != game->uptime_in_secs)
		{
			last_trigger[node] = game->uptime_in_secs;
			node->active = true;
		}
	}
};