#pragma once

#include <memory>
#include "Action.h"

#define RegisterBreakwallActionType(gamePtr, type, func)				\
	ActionHandler::GetInstance()->actions.register_action(				\
		type,															\
		[gamePtr]() {													\
			return func(*gamePtr);										\
		}																\
	)	
#define RemoveActionDispatcher(type)									\
	ActionHandler::GetInstance()->actions.delete_action(type)

#define ExecuteAction(type)												\
	ActionHandler::GetInstance()->actions.execute(type)

class ActionHandler
{
private:
	static std::unique_ptr<ActionHandler> h_instance;
public:
	ActionHandler() 
		: actions() {}
	static ActionHandler* GetInstance();
	ActionRegistry<Action_Type> actions;
};
