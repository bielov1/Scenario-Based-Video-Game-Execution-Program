#include "ActionHandler.h"


std::unique_ptr<ActionHandler> ActionHandler::h_instance = nullptr;

ActionHandler* ActionHandler::GetInstance()
{
	if (!h_instance)
		h_instance = std::make_unique<ActionHandler>();

	return h_instance.get();
}