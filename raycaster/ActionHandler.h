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

template<typename WorldMap, typename Node, typename Type, 
		 typename Func, typename... Args>
inline int RegisterAction(WorldMap* map, Node* node, Type type, 
						  Func func, Args... args) {
	return ActionHandler::GetInstance()->actions.register_action(
		type,
		[map, node, func, args...]() {
			return func(map, node, args...);
		}
	);
}

inline void DeleteAction(int id) {
	ActionHandler::GetInstance()->actions.delete_action(id);
}

inline void ExecuteAction(int id) {
	ActionHandler::GetInstance()->actions.execute_action(id);
}