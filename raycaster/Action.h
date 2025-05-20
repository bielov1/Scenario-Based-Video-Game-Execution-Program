#pragma once

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>

enum class Action_Type
{
	BREAKWALL
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
	std::map<T, func> registry;
public:
	void register_action(T type, const func& f)
	{
		registry.insert(std::make_pair(type, f));
	}

	void delete_action(T type)
	{
		auto it = registry.find(type);
		if (it != registry.end())
		{
			registry.erase(it);
		} else {
			std::cerr << "remove_dispatcher didn't work.";
		}
	}

	void execute(T type)
	{
		auto it = registry.find(type);
		if (it != registry.end())
		{
			it->second();
		} else {
			std::cerr << " dispatch didn't work.\n";
		}
	}
};