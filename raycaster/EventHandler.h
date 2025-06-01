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

template<typename Game, typename Node, typename Type, typename Func>
inline int RegisterEvent(Game* game, Node* node, Type type, Func func, std::string arg1, std::string arg2) {
	return (EventHandler::GetInstance()->events.register_event(
		type,
		[game, node, func, arg1, arg2]() {
			return func(game, node, arg1, arg2);
		}
	));
}

template<typename Game, typename Node, typename Type, typename Func>
inline int RegisterEvent(Game* game, Node* node, Type type, Func func, std::string arg1) {
	return (EventHandler::GetInstance()->events.register_event(
		type,
		[game, node, func, arg1]() {
			return func(game, node, arg1);
		}
	));
}

inline void DeleteEvent(int id) {
	EventHandler::GetInstance()->events.delete_event(id);
}

inline void ValidateEvent(int id) {
	EventHandler::GetInstance()->events.validate_event(id);
}