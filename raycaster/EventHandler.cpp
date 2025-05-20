#include "EventHandler.h"

std::unique_ptr<EventHandler> EventHandler::h_instance = nullptr;

EventHandler* EventHandler::GetInstance()
{
	if (!h_instance)
		h_instance = std::make_unique<EventHandler>();

	return h_instance.get();
}