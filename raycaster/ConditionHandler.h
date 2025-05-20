#pragma once

#include <memory>
#include "Condition.h"

#define RegisterAnyConditionType(gamePtr, type, func, event_type_arg)	\
	ConditionHandler::GetInstance()->conditions.register_condition(		\
		type,															\
		[gamePtr, event_type_arg]() {									\
			return func(*gamePtr, event_type_arg);						\
		}																\
	)	

#define DeleteConditionRegistery(type)									\
	ConditionHandler::GetInstance()->conditions.delete_condition(type)

#define ValidateCondition(type)											\
	ConditionHandler::GetInstance()->conditions.validate(type)
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
