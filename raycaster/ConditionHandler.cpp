#include "ConditionHandler.h"

std::unique_ptr<ConditionHandler> ConditionHandler::h_instance = nullptr;

ConditionHandler* ConditionHandler::GetInstance()
{
	if (!h_instance)
		h_instance = std::make_unique<ConditionHandler>();

	return h_instance.get();
}