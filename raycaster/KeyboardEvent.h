#pragma once
#include "Event.h"
class KeyboardUPEvent : public Event<Event_Type>
{
private:
public:
	KeyboardUPEvent() 
		: Event<Event_Type>("keyboard up", Event_Type::KEYBOARD_UP) {}
	static bool check(Game& game, std::string arg)
	{
		std::cout << "checking KeyboardUPEvent::check with argument: "
			<< arg
			<< std::endl;
		std::cout << "current Player property (moving): "
			<< game.playerInstance().moving
			<< std::endl;
		std::cout << "changing Player.moving to: "
			<< arg
			<< std::endl;

		game.playerInstance().moving = arg;

		std::cout << "current Player property (moving): "
			<< game.playerInstance().moving
			<< std::endl;
		return true;
	}
};

class KeyboardDOWNEvent : public Event<Event_Type>
{
private:
public:
	KeyboardDOWNEvent() 
		: Event<Event_Type>("keyboard down", Event_Type::KEYBOARD_DOWN) {}
	static bool check(Game& game, std::string arg)
	{
		std::cout << "checking KeyboardDOWNEvent::check with argument: "
			<< arg
			<< std::endl;
		std::cout << "current Player property (moving): "
			<< game.playerInstance().moving
			<< std::endl;
		std::cout << "changing Player.moving to: "
			<< arg
			<< std::endl;

		game.playerInstance().moving = arg;

		std::cout << "current Player property (moving): "
			<< game.playerInstance().moving
			<< std::endl;
		return true;
	}
};

class KeyboardLEFTEvent : public Event<Event_Type>
{
private:
public:
	KeyboardLEFTEvent() 
		: Event<Event_Type>("keyboard left", Event_Type::KEYBOARD_LEFT) {}
	static bool check(Game& game, std::string arg)
	{
		std::cout << "checking KeyboardLEFTEvent::check with argument: "
			<< arg
			<< std::endl;
		std::cout << "current Player property (moving): "
			<< game.playerInstance().moving
			<< std::endl;
		std::cout << "changing Player.moving to: "
			<< arg
			<< std::endl;

		game.playerInstance().moving = arg;

		std::cout << "current Player property (moving): "
			<< game.playerInstance().moving
			<< std::endl;
		return true;
	}
};

class KeyboardRIGHTEvent : public Event<Event_Type>
{
private:
public:
	KeyboardRIGHTEvent() 
		: Event<Event_Type>("keyboard right", Event_Type::KEYBOARD_RIGHT) {}
	static void check(Game& game, std::string arg)
	{
		std::cout << "checking KeyboardRIGHTEvent::check with argument: "
			<< arg
			<< std::endl;
		std::cout << "current Player property (moving): "
			<< game.playerInstance().moving
			<< std::endl;
		std::cout << "changing Player.moving to: "
			<< arg
			<< std::endl;

		game.playerInstance().moving = arg;

		std::cout << "current Player property (moving): "
			<< game.playerInstance().moving
			<< std::endl;
	}
};
