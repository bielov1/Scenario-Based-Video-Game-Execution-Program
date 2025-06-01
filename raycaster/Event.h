#pragma once

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>

enum class Event_Type
{
	INTERACTION,
	OBJECT,
	KEYBOARD_UP,
	KEYBOARD_DOWN,
	KEYBOARD_LEFT,
	KEYBOARD_RIGHT
};

template <typename T>
class Event
{
private:
	std::string name;
	T e_type;
public:
	Event(std::string n, T type)
		: name(n), e_type(type) {}
	T get_type() const
	{ return e_type; }
	std::string get_name() const
	{ return name; }
};

template <typename T>
class EventRegistry
{
private:
	using func = std::function<void()>;
	std::map<int, std::pair<T, func>> registry;
	int handle_id = 0;
public:
	int register_event(T type, const func& f)
	{
		int id = handle_id++;
		registry[id] = {type, f};
		return id;
	}

	void delete_event(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			registry.erase(it);
		} else {
			std::cerr << "remove_listener didn't work.";
		}
	}

	void validate_event(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			it->second.second();
		} else {
			std::cerr << " dispatch didn't work.\n";
		}
	}
};

