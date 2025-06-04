#pragma once
#include <memory>
#include "Event.h"

class EventHandler
{
private:
	static std::unique_ptr<EventHandler> h_instance;
public:
	EventHandler() 
		: events() {}
	static EventHandler* GetInstance();
	EventRegistry<Event_Type> events;
};


template<typename Game, typename Node, typename Type, typename Func, typename... Args>
inline int RegisterEvent(Game* game, Node* node, Type type, Func func, const std::vector<std::string>& raw_args, Args... args) {
	return EventHandler::GetInstance()->events.register_event(
		type,
		[game, node, func, args...]() {
			return func(game, node, args...);
		},
		raw_args
	);
}

inline void DeleteEvent(int id) {
	EventHandler::GetInstance()->events.delete_event(id);
}

inline void ValidateEvent(int id) {
	EventHandler::GetInstance()->events.validate_event(id);
}

inline Event_Type GetEventTypeByID(int id) {
	return EventHandler::GetInstance()->events.get_type_by_id(id);
}

inline std::string GetEventSecondArgById(int id) {
	return EventHandler::GetInstance()->events.get_second_arg_by_id(id);
}