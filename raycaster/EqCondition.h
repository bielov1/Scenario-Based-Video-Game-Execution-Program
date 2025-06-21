#pragma once
#include "Condition.h"

class EqCondition : public Condition<Cond_Type>
{
public:
	EqCondition() 
		: Condition<Cond_Type>("eq", Cond_Type::EQ) {}
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

	static void is_color(WorldMap* map, Node* node, std::string parsed_arg2)
	{
		if (parsed_arg2 == "blue") {
			if (map->wall_interaction->rgb_color == RGB(255, 0, 0)) {
				node->active = true;
			}
		} else if (parsed_arg2 == "green") {
			if (map->wall_interaction->rgb_color == RGB(0, 255, 0)) {
				node->active = true;
			}
		} else if (parsed_arg2 == "red") {
			if (map->wall_interaction->rgb_color == RGB(0, 0, 255)) {
				node->active = true;
			}
		}
	}

	static void validate(WorldMap* map, Node* node, std::string arg1, std::string arg2)
	{
		const Node* root = get_root(node);
		int numeric_arg2 = 0;
		int numeric_arg1 = 0;

		if (is_number(arg2)) {
			numeric_arg2 = std::stoi(arg2);
		}

		if (is_number(arg1)) {
			numeric_arg1 = std::stoi(arg1);
		}

		const std::string prefix_this1 = "this1.";
		const std::string prefix_this2 = "this2.";
		const std::string prefix_wall = "wall.";
		const std::string prefix_play = "player.";
		const std::string prefix_here = "here.";
		const std::string prefix_timer = "timer.";
		const std::string prefix_map = "map.";

		std::string root_arg2 = GetEventSecondArgById(root->id);
		std::string root_arg1 = GetEventFirstArgById(root->id);
		if (arg1.rfind(prefix_this2, 0) == 0) {
			if (root_arg2 == "player") {
				return;
			} else if (root_arg2 == "wall") {
				std::string after_dot = arg1.substr(prefix_this2.length());
				if (after_dot == "color") {
					is_color(map, node, arg2);
				}
			}
		} 
		else if (arg2.rfind(prefix_this2, 0) == 0) {
			if (root_arg2 == "player") {
				return;
			} else if (root_arg2 == "wall") {
				std::string after_dot = arg2.substr(prefix_this2.length());
				if (after_dot == "color") {
					is_color(map, node, arg1);
				}
			}
		}
		else if (arg1.rfind(prefix_this1, 0) == 0) {
			if (root_arg1 == "player") {
				return;
			} else if (root_arg1 == "wall") {
				std::string after_dot = arg1.substr(prefix_this1.length());
				if (after_dot == "color") {
					is_color(map, node, arg2);
				}
			}
		}
		else if (arg2.rfind(prefix_this1, 0) == 0) {
			if (root_arg1 == "player") {
				return;
			} else if (root_arg1 == "wall") {
				std::string after_dot = arg2.substr(prefix_this1.length());
				if (after_dot == "color") {
					is_color(map, node, arg1);
				}
			}
		}
		else if (arg1.rfind(prefix_wall, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_wall.length());
			if (after_dot == "color") {
				is_color(map, node, arg2);
			}
		} 
		else if (arg2.rfind(prefix_wall, 0) == 0) {
			std::string after_dot = arg2.substr(prefix_wall.length());
			if (after_dot == "color") {
				is_color(map, node, arg1);
			}
		} 
		else if (arg1.rfind(prefix_here, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_here.length());
			Event_Type t = GetEventTypeByID(root->id);
			if (t == Event_Type::TIMER) {
				if (after_dot == "time") {
					if (map->quest_timer.get_timer_count() == numeric_arg2) {
						node->active = true;
					}
				} 
			}
		} 
		else if (arg1.rfind(prefix_timer, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_timer.length());
			Event_Type t = GetEventTypeByID(root->id);
			if (t == Event_Type::TIMER) {
				if (after_dot == "time") {
					if (map->quest_timer.get_timer_count() == numeric_arg2) {
						node->active = true;
					}
				} 
			}
		} 
		else if (arg2.rfind(prefix_here, 0) == 0) {
			std::string after_dot = arg2.substr(prefix_here.length());
			Event_Type t = GetEventTypeByID(root->id);
			if (t == Event_Type::TIMER) {
				if (after_dot == "time") {
					if (map->quest_timer.get_timer_count() == numeric_arg1) {
						node->active = true;
					}
				} 
			}
		} 
		else if (arg2.rfind(prefix_timer, 0) == 0) {
			std::string after_dot = arg2.substr(prefix_timer.length());
			Event_Type t = GetEventTypeByID(root->id);
			if (t == Event_Type::TIMER) {
				if (after_dot == "time") {
					if (map->quest_timer.get_timer_count() == numeric_arg1) {
						node->active = true;
					}
				} 
			}
		}
		else if (arg1.rfind(prefix_map, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_map.length());
			if (after_dot == "empty") {
				if (arg2 == "true") {
					if (map->map_empty()) {
						node->active = true;
					}
				} else if (arg2 == "false") {
					if (!map->map_empty()) {
						node->active = true;
					}
				}
			}
		}
		else if (arg2.rfind(prefix_map, 0) == 0) {
			std::string after_dot = arg2.substr(prefix_map.length());
			if (after_dot == "empty") {
				if (arg1 == "true") {
					if (map->map_empty()) {
						node->active = true;
					}
				} else if (arg1 == "false") {
					if (!map->map_empty()) {
						node->active = true;
					}
				}
			}
		}
	}
};
