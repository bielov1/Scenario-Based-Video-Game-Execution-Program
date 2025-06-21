#pragma once
#include "Condition.h"

class NeqCondition : public Condition<Cond_Type>
{
public:
	NeqCondition() 
		: Condition<Cond_Type>("neq", Cond_Type::NEQ) {}
	static const Node* get_root(const Node* node)
	{
		while (node && node->prev_node) {
			node = node->prev_node;
		}
		return node;
	}

	static bool is_number(const std::string& s) {
		if (s.empty()) return false;

		size_t start = 0;
		if (s[0] == '-' || s[0] == '+') {
			if (s.length() == 1) return false;
			start = 1;
		}

		for (size_t i = start; i < s.length(); i++) {
			if (!std::isdigit(s[i])) return false;
		}
		return true;
	}

	static void validate(WorldMap* map, Node* node, std::string arg1, std::string arg2)
	{
		return;
	}
};
