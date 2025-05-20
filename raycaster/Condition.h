#pragma once

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>

enum class Cond_Type
{
	ANY,
	STRUCTURED
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
	std::map<T, func> registry;
public:
	void register_condition(T type, const func& f)
	{
		registry.insert(std::make_pair(type, f));
	}

	void delete_condition(T type)
	{
		auto it = registry.find(type);
		if (it != registry.end())
		{
			registry.erase(it);
		} else {
			std::cerr << "remove_registry didn't work.";
		}
	}

	void validate(T type)
	{
		auto it = registry.find(type);
		if (it != registry.end())
		{
			it->second();
		} else {
			std::cerr << " validate didn't work.\n";
		}
	}
};


