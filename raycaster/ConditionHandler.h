#pragma once

#include <memory>
#include "Condition.h"

class ConditionHandler
{
private:
	static std::unique_ptr<ConditionHandler> h_instance;
public:
	ConditionHandler() 
		: conditions() {}
	static ConditionHandler* GetInstance();
	ConditionRegistry<Cond_Type> conditions;
};

template<typename Game, typename Node, typename Type, typename Func>
inline int RegisterCondition(Game* game, Node* node, Type type, Func func) {
	return (ConditionHandler::GetInstance()->conditions.register_condition(
		type,
		[game, node, func]() {
			return func(game, node);
		}
	));
}

inline void DeleteCondition(int id) {
	ConditionHandler::GetInstance()->conditions.delete_condition(id);
}

inline void ValidateCondition(int id) {
	ConditionHandler::GetInstance()->conditions.validate_condition(id);
}