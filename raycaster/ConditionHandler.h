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

template<typename WorldMap, typename Node, typename Type, 
		 typename Func, typename... Args>
inline int RegisterCondition(WorldMap* map, Node* node, Type type, 
							 Func func, Args... args) {
	return ConditionHandler::GetInstance()->conditions.register_condition(
		type,
		[map, node, func, args...]() {
			return func(map, node, args...);
		}
	);
}

inline void DeleteCondition(int id) {
	ConditionHandler::GetInstance()->conditions.delete_condition(id);
}

inline void ValidateCondition(int id) {
	ConditionHandler::GetInstance()->conditions.validate_condition(id);
}