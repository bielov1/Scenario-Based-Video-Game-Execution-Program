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
	
	static void validate(WorldMap* map, Node* node, std::string arg1, std::string arg2)
	{
		const Node* root = get_root(node);
		std::string parsed_arg2 = arg2;
		int numeric_arg2 = 0;

		if (is_number(arg2)) {
			numeric_arg2 = std::stoi(arg2);
		}

		const std::string prefix_greenwalls = "greenwalls.";
		const std::string prefix_redwalls = "redwalls.";
		const std::string prefix_bluewalls = "bluewalls.";
		const std::string prefix_this = "this2.";
		const std::string prefix_here = "here.";
		if (arg1.rfind(prefix_this, 0) == 0) {
			std::string root_arg2 = GetEventSecondArgById(root->id);
			std::string root_arg1 = GetEventFirstArgById(root->id);
			if (root_arg1 == "player") {
				int player_pos_x = static_cast<int>(floor(map->player.pos.x));
				int player_pos_y = static_cast<int>(floor(map->player.pos.y));

				if (root_arg2 == "wall") {
					std::string after_dot = arg1.substr(prefix_this.length());
					if (after_dot == "color") {
						if (parsed_arg2 == "blue") {
							size_t blue_walls_count = map->blue_walls.size();
							for (int i = 0; i < blue_walls_count; i++) {
								int blue_wall_pos_x = static_cast<int>(map->blue_walls[i]->pos.x);
								int blue_wall_pos_y = static_cast<int>(map->blue_walls[i]->pos.y);
								if (player_pos_x == blue_wall_pos_x && player_pos_y == blue_wall_pos_y &&
									map->blue_walls[i]->render) {
									node->active = true;
									return;
								}
							}
						} else if (parsed_arg2 == "green") {
							size_t green_walls_count = map->green_walls.size();
							for (int i = 0; i < green_walls_count; i++) {
								int green_wall_pos_x = static_cast<int>(map->green_walls[i]->pos.x);
								int green_wall_pos_y = static_cast<int>(map->green_walls[i]->pos.y);
								if (player_pos_x == green_wall_pos_x && player_pos_y == green_wall_pos_y &&
									map->green_walls[i]->render) {
									node->active = true;
									return;
								}
							}
						} else if (parsed_arg2 == "red") {
							size_t red_walls_count = map->red_walls.size();
							for (int i = 0; i < red_walls_count; i++) {
								int red_wall_pos_x = static_cast<int>(map->red_walls[i]->pos.x);
								int red_wall_pos_y = static_cast<int>(map->red_walls[i]->pos.y);
								if (player_pos_x == red_wall_pos_x && player_pos_y == red_wall_pos_y &&
									map->red_walls[i]->render) {
									node->active = true;
									return;
								}
							}
						}
					}
 				}
			} 
		} else if (arg1.rfind(prefix_greenwalls, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_greenwalls.length());
			if (after_dot == "count") {
				if (map->greenwalls_count_equals(numeric_arg2)) {
					node->active = true;
				}
			}
		} else if (arg1.rfind(prefix_bluewalls, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_bluewalls.length());
			if (after_dot == "count") {
				if (map->bluewalls_count_equals(numeric_arg2)) {
					node->active = true;
				}
			}
		} else if (arg1.rfind(prefix_redwalls, 0) == 0) {
			std::string after_dot = arg1.substr(prefix_redwalls.length());
			if (after_dot == "count") {
				if (map->redwalls_count_equals(numeric_arg2)) {
					node->active = true;
				}
			}
		} else if (arg1.rfind(prefix_here, 0) == 0) {
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
	}
};
