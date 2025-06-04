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
	MAP,
	TIMER
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

	struct EventEntry {
		T type;
		func f;
		std::vector<std::string> args;
	};
	std::map<int, EventEntry> registry;
	int handle_id = 0;
public:
	int register_event(T type, const func& f, const std::vector<std::string>& args = {})
	{
		int id = handle_id++;
		registry[id] = {type, f, args};
		return id;
	}

	void delete_event(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			registry.erase(it);
		} else {
			throw std::out_of_range("delete_event: Invalid event ID");
		}
	}

	void validate_event(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			it->second.f();
		} else {
			throw std::out_of_range("validate_event: Invalid event ID");
		}
	}

	T get_type_by_id(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			return it->second.type;
		}
		throw std::out_of_range("get_type_by_id: Invalid event ID");
	}

	std::string get_second_arg_by_id(int id) {
		auto it = registry.find(id);
		if (it != registry.end() && it->second.args.size() >= 2) {
			return it->second.args[1];
		}
		throw std::out_of_range("Invalid ID or not enough arguments");
	}
};

