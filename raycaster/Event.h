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
class EventDispatcher
{
private:
	using func = std::function<void()>;
	std::map<T, func> listener;
public:
	void add_listener(T type, const func& f)
	{
		listener.insert(std::make_pair(type, f));
	}

	void remove_listener(T type)
	{
		auto it = listener.find(type);
		if (it != listener.end())
		{
			listener.erase(it);
		} else {
			std::cerr << "remove_listener didn't work.";
		}
	}

	void dispatch(T type)
	{
		auto it = listener.find(type);
		if (it != listener.end())
		{
			it->second();
		} else {
			std::cerr << " dispatch didn't work.\n";
		}
	}
};

