#pragma once
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include "WorldMap.h"

enum class Action_Type
{
	BREAKWALL,
	SUB,
	FAILED,
	VICTORY,
	BUILD,
	SHOWALL,
	REMOVE,
	SET
};

template <typename T>
class Action
{
private:
	std::string name;
	T a_type;
public:
	Action(std::string n, T type)
		: name(n), a_type(type) {}
	T get_type() const
	{ return a_type; }
	std::string get_name() const
	{ return name; }
};

template <typename T>
class ActionRegistry
{
private:
	using func = std::function<void()>;
	std::map<int, std::pair<T, func>> registry;
	int register_id = 0;
public:
	int register_action(T type, const func& f)
	{
		int id = register_id++;
		registry[id] = {type, f};
		return id;
	}

	void delete_action(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			registry.erase(it);
		} else {
			throw std::out_of_range("delete_action: Invalid action ID.");
		}
	}

	void execute_action(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			it->second.second();
		} else {
			throw std::out_of_range("execute_action: Invalid action ID");
		}
	}
};