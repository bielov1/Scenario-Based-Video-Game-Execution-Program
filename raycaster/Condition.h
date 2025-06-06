#pragma once
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include "WorldMap.h"

class Game;

enum class Cond_Type
{
	ANY,
	NONE,
	EQ
};

template <typename T>
class Condition
{
private:
	std::string name;
	T c_type;
public:
	Condition(std::string n, T type)
		: name(n), c_type(type) {}
	T get_type() const
	{ return c_type; }
	std::string get_name() const
	{ return name; }
};

template <typename T>
class ConditionRegistry
{
private:
	using func = std::function<void()>;
	std::map<int, std::pair<T, func>> registry;
	int register_id = 0;
public:
	int register_condition(T type, const func& f)
	{
		int id = register_id++;
		registry[id] = {type, f};
		return id;
	}

	void delete_condition(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			registry.erase(it);
		} else {
			std::cerr << "remove_registry didn't work.";
		}
	}

	void validate_condition(int id)
	{
		auto it = registry.find(id);
		if (it != registry.end())
		{
			it->second.second();
		} else {
			std::cerr << " validate didn't work.\n";
		}
	}
};


