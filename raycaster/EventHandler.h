#pragma once
#include <memory>
#include "Event.h"

#define AddInteractionEventListener(gamePtr, type, func, arg1, arg2)	\
	EventHandler::GetInstance()->events.add_listener(					\
		type,															\
		[gamePtr, arg1, arg2]() {										\
			return func(*gamePtr, arg1, arg2);							\
		}																\
	)	

#define AddObjectEventListener(gamePtr, type, func, arg1)				\
	EventHandler::GetInstance()->events.add_listener(					\
		type,															\
		[gamePtr, arg1]() {												\
			return func(*gamePtr, arg1);								\
		}																\
	)

#define AddKeyboardEventListener(gamePtr,type, func, arg1)				\
	EventHandler::GetInstance()->events.add_listener(					\
		type,															\
		[gamePtr, arg1]() {												\
			return func(*gamePtr, arg1);								\
		}																\
	)

#define RemoveEventListener(type) \
	EventHandler::GetInstance()->events.remove_listener(type)

#define DispatchEvent(type) \
	EventHandler::GetInstance()->events.dispatch(type)


class EventHandler
{
private:
	static std::unique_ptr<EventHandler> h_instance;
public:
	EventHandler() 
		: events() {}
	static EventHandler* GetInstance();
	EventDispatcher<Event_Type> events;
};