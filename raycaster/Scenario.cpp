#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include "Scenario.h"
#include "InteractionEvent.h"
#include "MapEvent.h"
#include "TimerEvent.h"
#include "AnyCondition.h"
#include "NoneCondition.h"
#include "EqCondition.h"
#include "BreakwallAction.h"
#include "SubAction.h"
#include "FailedAction.h"
#include "VictoryAction.h"
#include "BuildAction.h"
#include "ShowallAction.h"
#include "RemoveAction.h"
#include "SetAction.h"

Scenario::Scenario()
	: scenario_path(""), content(""), tokens() {}

void Scenario::init(std::string path)
{
	scenario_path = path;
	get_content();
	lexer.init(content, static_cast<std::size_t>(content.length()));
	Token t;
	for(;;) {
		t = lexer.next_token();
		if (t.kind == Token_Kind::END) break;
		tokens.push_back(t);
	}
	parse();
	validate_branches();
	reset_branches();

}

void Scenario::get_content()
{
	std::ifstream scen(scenario_path);
	std::string line;
	while (std::getline(scen, line)) {
		if (line.empty() || (line[0] == '/' && line[1] == '/')) continue;
		content += line;
		content.push_back('\n');
	}
}

void Scenario::execute_actions(Node* action_branch)
{
	Node* act_node = action_branch;
	while (act_node != nullptr) {
		ExecuteAction(act_node->id);
		act_node = act_node->lact_node;
	}
}

void Scenario::reset_branches()
{
	for (const Scenario_Branch branch : branches) {
		Node* node = branch;
		while (node != nullptr) {
			node->active = false;
			node = node->rcond_node;
		}
	}
}

void Scenario::validate_branches()
{
	std::stack<Node*> stack;
	for (const Scenario_Branch event : branches) {
		ValidateEvent(event->id);
		if (event->active) {
			Node* condition = event->rcond_node;
			while (condition != nullptr) {
				ValidateCondition(condition->id);
				stack.push(condition);
				if (condition->lact_node) {
					bool all_conds_active = true;
					while (!stack.empty()) {
						if (!stack.top()->active) {
							all_conds_active = false;
						}
						stack.pop();
					}

					if (condition->active && all_conds_active) {
						execute_actions(condition->lact_node);
					}
				}
				condition = condition->rcond_node;
			}
		}
	}
}

void Scenario::add_node_to_branch(Scenario_Branch branch, Branch_Node node)
{
	if (branch == nullptr || node == nullptr)
		return;

	Node* next = branch;
	if (node->level == Scenario_Level::CONDITION) {
		for (;next->rcond_node != nullptr; next = next->rcond_node);
		next->rcond_node = node;
		next->rcond_node->prev_node = next;
	} else if (node->level == Scenario_Level::ACTION) {
		for (;next->rcond_node != nullptr; next = next->rcond_node);
		for (;next->lact_node != nullptr; next = next->lact_node);
		next->lact_node = node;
		next->lact_node->prev_node = next;
	}
}

template<typename Func, typename... Args>
void Scenario::register_event(Event_Type type, Func func, Args... args)
{
	std::vector<std::string> raw_args = { args... };
	Scenario_Branch event_branch = new Node();
	int event_id = RegisterEvent(&world_map, event_branch, type, func, raw_args, args...);
	event_branch->id = event_id;
	event_branch->active = false;
	event_branch->level = Scenario_Level::EVENT;
	branches.push_back(event_branch);
}

template<typename Func, typename... Args>
void Scenario::register_condition(Cond_Type type, Func func, Args... args)
{
	Scenario_Branch last_parsed_branch = branches.back();
	Branch_Node cond_node = new Node();
	int condition_id = RegisterCondition(&world_map, cond_node, type, func, args...);
	cond_node->id = condition_id;
	cond_node->active = false;
	cond_node->level = Scenario_Level::CONDITION;
	add_node_to_branch(last_parsed_branch, cond_node);
}

template<typename Func, typename... Args>
void Scenario::register_action(Action_Type type, Func func, Args... args)
{
	Scenario_Branch last_parsed_branch = branches.back();
	Branch_Node act_node = new Node();
	int act_id = RegisterAction(&world_map, act_node, type, func, args...);
	act_node->id = act_id;
	act_node->active = true;
	act_node->level = Scenario_Level::ACTION;
	add_node_to_branch(last_parsed_branch, act_node);
}

void Scenario::parse()
{
	std::size_t i = 0;
	while (i < tokens.size())
	{
		auto& t = tokens[i];

		if (t.kind == Token_Kind::EVENT_INTERACTION) {
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			i += 3;
			register_event(Event_Type::INTERACTION, InteractionEvent::check, arg1, arg2);
		} else if (t.kind == Token_Kind::EVENT_MAP) {
			i += 1;
			register_event(Event_Type::MAP, MapEvent::check);
		} else if (t.kind == Token_Kind::EVENT_TIMER) {
			std::string arg = tokens[i+1].text;
			i += 2;
			register_event(Event_Type::TIMER, TimerEvent::check, arg);
		} else if (t.kind == Token_Kind::COND_ANY) {
			++i;
			register_condition(Cond_Type::ANY, AnyCondition::validate);
		} else if (t.kind == Token_Kind::COND_NONE) {
			++i;
			register_condition(Cond_Type::NONE, NoneCondition::validate);
		} else if (t.kind == Token_Kind::COND_EQ) {
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			i += 3;
			register_condition(Cond_Type::EQ, EqCondition::validate, arg1, arg2);
		} else if (t.kind == Token_Kind::ACTION_BREAKWALL) {
			std::string arg = tokens[i+1].text;
			i += 2;
			register_action(Action_Type::BREAKWALL, BreakwallAction::act, arg);
		} else if (t.kind == Token_Kind::ACTION_SUB) {
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			i += 3;
			register_action(Action_Type::SUB, SubAction::act, arg1, arg2);
		} else if (t.kind == Token_Kind::ACTION_FAILED) {
			i += 1;
			register_action(Action_Type::FAILED, FailedAction::act);
		} else if (t.kind == Token_Kind::ACTION_VICTORY) {
			i += 1;
			register_action(Action_Type::VICTORY, VictoryAction::act);
		} else if (t.kind == Token_Kind::ACTION_SET) {
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			i += 3;
			register_action(Action_Type::SET, SetAction::act, arg1, arg2);
		} else if (t.kind == Token_Kind::ACTION_BUILD) {
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			std::string arg3 = tokens[i+3].text;
			std::string arg4 = tokens[i+4].text;
			i += 5;
			register_action(Action_Type::BUILD, BuildAction::act, arg1, arg2, arg3, arg4);
		} else if (t.kind == Token_Kind::ACTION_SHOWALL) {
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			i += 3;
			register_action(Action_Type::SHOWALL, ShowallAction::act, arg1, arg2);
		} else if (t.kind == Token_Kind::ACTION_REMOVE) {
			std::string arg1 = tokens[i+1].text;
			i += 2;
			register_action(Action_Type::REMOVE, RemoveAction::act, arg1);
		} else {
			throw std::out_of_range("parse: uknown kind of token");
		}

	}
}