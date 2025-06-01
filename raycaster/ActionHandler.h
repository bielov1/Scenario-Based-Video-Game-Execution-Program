#pragma once

#include <memory>
#include "Action.h"
	
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

template<typename Game, typename Type, typename Func>
inline int RegisterAction(Game* game, Type type, Func func, std::string arg) {
	return (ActionHandler::GetInstance()->actions.register_action(
		type,
		[game, func, arg]() {
			return func(game, arg);
		}
	));
}

inline void DeleteAction(int id) {
	ActionHandler::GetInstance()->actions.delete_action(id);
}

inline void ExecuteAction(int id) {
	ActionHandler::GetInstance()->actions.execute_action(id);
}