#pragma once
#include "Event.h"

class TimerEvent : public Event<Event_Type>
{
public:
	TimerEvent() 
		: Event<Event_Type>("timer", Event_Type::TIMER) {}
	static void check(WorldMap* map, Node* node, std::string arg)
	{
		static std::map<Node*, int> last_trigger;
		int interval = std::stoi(arg);
		if (interval <= 0) return;

		if (map->uptime_in_secs % interval == 0 &&
			last_trigger[node] != map->uptime_in_secs)
		{
			last_trigger[node] = map->uptime_in_secs;
			node->active = true;
		}
	}
};