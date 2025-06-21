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
#include "NeqCondition.h"
#include "ActiveCondition.h"
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
	tokens.clear();
	content.clear();
	validation_errors.clear();
	command_args.clear();
	expected_command_types.clear();
	
	init_com_args();
	init_expected_types();

	get_content();
	lexer.init(content, static_cast<std::size_t>(content.length()));
	Token t;
	for(;;) {
		t = lexer.next_token();
		if (t.kind == Token_Kind::END) break;
		tokens.push_back(t);
	}
}

void Scenario::get_content()
{
	std::ifstream scen(scenario_path);
	std::string line;
	while (std::getline(scen, line)) {
		if (line[0] == '/' && line[1] == '/') continue;
		content += line;
		content.push_back('\n');
	}
}

void Scenario::init_com_args()
{
	command_args[Token_Kind::EVENT_INTERACTION] = 2;
	command_args[Token_Kind::EVENT_MAP] = 0;
	command_args[Token_Kind::EVENT_TIMER] = 1;
	command_args[Token_Kind::COND_ANY] = 0;
	command_args[Token_Kind::COND_NONE] = 0;
	command_args[Token_Kind::COND_EQ] = 2;
	command_args[Token_Kind::COND_NEQ] = 2;
	command_args[Token_Kind::COND_ACTIVE] = 1;
	command_args[Token_Kind::ACTION_SUB] = 2;
	command_args[Token_Kind::ACTION_BREAKWALL] = 0;
	command_args[Token_Kind::ACTION_SET] = 2;
	command_args[Token_Kind::ACTION_FAILED] = 0;
	command_args[Token_Kind::ACTION_VICTORY] = 0;
	command_args[Token_Kind::ACTION_BUILD] = 4;
	command_args[Token_Kind::ACTION_SHOWALL] = 2;
	command_args[Token_Kind::ACTION_REMOVE] = 1;
}

void Scenario::init_expected_types()
{
	expected_command_types[Token_Kind::EVENT_INTERACTION] = {Argument_Type::STRING, Argument_Type::STRING};
	expected_command_types[Token_Kind::EVENT_MAP] = {};
	expected_command_types[Token_Kind::EVENT_TIMER] = {Argument_Type::INTEGER};
	expected_command_types[Token_Kind::COND_ANY] = {};
	expected_command_types[Token_Kind::COND_NONE] = {};
	expected_command_types[Token_Kind::COND_EQ] = {Argument_Type::STRING, Argument_Type::MIXED};
	expected_command_types[Token_Kind::COND_NEQ] = {Argument_Type::STRING, Argument_Type::MIXED};
	expected_command_types[Token_Kind::COND_ACTIVE] = {Argument_Type::STRING};
	expected_command_types[Token_Kind::ACTION_SUB] = {Argument_Type::STRING, Argument_Type::INTEGER};
	expected_command_types[Token_Kind::ACTION_BREAKWALL] = {};
	expected_command_types[Token_Kind::ACTION_SET] = {Argument_Type::STRING, Argument_Type::INTEGER};
	expected_command_types[Token_Kind::ACTION_FAILED] = {};
	expected_command_types[Token_Kind::ACTION_VICTORY] = {};
	expected_command_types[Token_Kind::ACTION_BUILD] = {Argument_Type::STRING, Argument_Type::STRING, Argument_Type::INTEGER, Argument_Type::INTEGER};
	expected_command_types[Token_Kind::ACTION_SHOWALL] = {Argument_Type::STRING, Argument_Type::STRING};
	expected_command_types[Token_Kind::ACTION_REMOVE] = {Argument_Type::STRING};
}

bool Scenario::is_valid_number(const std::string& str)
{
	if (str.empty()) return false;

	std::string::const_iterator it = str.begin();

	if (*it == '-') {
		++it;
		if (it == str.end()) return false;
	}

	while (it != str.end() && std::isdigit(*it)) ++it;

	return it == str.end();
}

bool Scenario::starts_with_char(const char& x)
{
	return std::isalpha(static_cast<unsigned char>(x));
}

bool Scenario::validate_argument_type(const Token& arg, int arg_pos, Argument_Type expected_type)
{
	switch(expected_type) {
	case Argument_Type::STRING:
		return starts_with_char(arg.text[0]);
	case Argument_Type::INTEGER:
	{
		if (!is_valid_number(arg.text)) {
			return false;
		}

		int value = std::stoi(arg.text);
		if (value < 0) {
			validation_errors.push_back(Validation_Error(
				"ERROR: argument '" + arg.text + "' at position " +
				std::to_string(arg_pos + 1) + " must be >= 0.",
				arg.line,
				arg.text
			));
		}
		return true;
	}
	case Argument_Type::MIXED:
	{
		if (is_valid_number(arg.text)) {
			int value = std::stoi(arg.text);
			if (value < 0) {
				validation_errors.push_back(Validation_Error(
					"ERROR: argument '" + arg.text + "' at position " +
					std::to_string(arg_pos + 1) + " must be >= 0.",
					arg.line,
					arg.text
				));
			}
			return true;
		}
			
		return starts_with_char(arg.text[0]);
	}
	default:
		return false;
	}
}

bool Scenario::validate_command_args(const Token& command, size_t& command_index)
{
	if (command_index + 1 >= tokens.size()) return false;

	auto args_it = command_args.find(command.kind);
	if (args_it == command_args.end()) {
		return false;
	}

	auto types_it = expected_command_types.find(command.kind);
	if (types_it == expected_command_types.end()) {
		return false;
	}

	int expected_args = args_it->second;
	const std::vector<Argument_Type>& expected_types = types_it->second;

	int actual_args = 0;
	int fixate_command_line = command.line;
	size_t next_command_arg_index = command_index + 1;

	std::vector<Token> arguments;
	while(next_command_arg_index < tokens.size() && 
		  tokens[next_command_arg_index].line == fixate_command_line &&
		  tokens[next_command_arg_index].kind == Token_Kind::ARGUMENT) {
		arguments.push_back(tokens[next_command_arg_index]);
		actual_args++;
		next_command_arg_index++;
	}


	if (actual_args < expected_args) {
		validation_errors.push_back(Validation_Error(
			"ERROR: command '" + command.text + "' has not enought amount of arguments" +
			". Expected: " + std::to_string(expected_args) +
			", got: " + std::to_string(actual_args),
			command.line,
			command.text
		));
		command_index += actual_args;
		return false;
	} else if (actual_args > expected_args) {
		validation_errors.push_back(Validation_Error(
			"ERROR: command '" + command.text + "' has too many arguments" +
			". Expected: " + std::to_string(expected_args) +
			", got: " + std::to_string(actual_args),
			command.line,
			command.text
		));
		command_index += actual_args;
		return false;
	}

	bool type_errors_found = false;
	for (int i = 0; i < actual_args; ++i) {
		if (!validate_argument_type(arguments[i], i, expected_types[i])) {
			std::string type_name;
			switch (expected_types[i]) {
			case Argument_Type::STRING: 
				type_name = "string";
				break;
			case Argument_Type::INTEGER: 
				type_name = "number";
				break;
			}

			validation_errors.push_back(Validation_Error(
				"ERROR: command '" + command.text + "' argument at position " + std::to_string(i + 1) +
				" has wrong type. Expected: " + type_name +
				", got: '" + arguments[i].text + "'",
				command.line,
				arguments[i].text
			));

		}
	}

	command_index = next_command_arg_index;
	return !type_errors_found;
}

bool Scenario::validate_structure()
{
	std::size_t i = 0;

	while (i < tokens.size()) {
		Token& token = tokens[i];

		if (token.kind != Token_Kind::ARGUMENT) {
			if (!validate_command_args(token, i)) {
				i++;
			}
		} else {
			// mistake in command name
			validation_errors.push_back(Validation_Error(
				"ERROR: Incorrect command name: '" + token.text + "'",
				token.line,
				token.text
			));
			while (i < tokens.size()) {
				if (tokens[i].kind != Token_Kind::ARGUMENT) {
					break;
				}
				i++;
			}
		}
	}

	return validation_errors.empty();
}

std::string Scenario::get_validation_report()
{
	std::string report = "Error count: " + std::to_string(validation_errors.size()) + "\n\n";

	for (const auto& error : validation_errors) {
		report += "line: " + std::to_string(error.line + 1) + ": " +
			error.message;
		report += "\n";
	}

	return report;
}

std::string Scenario::form_scenario_validation_report()
{
	validation_errors.clear();
	std::string error_report = "";
	if (tokens.empty()) {
		validation_errors.push_back(Validation_Error(
			"ERROR: no tokens to process",
			0,
		    ""
		));
		error_report = get_validation_report();
		return error_report;
	}

	bool is_valid = validate_structure();

	if (!is_valid) {
		error_report = get_validation_report();	
	}

	return error_report;
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
		} else if (t.kind == Token_Kind::COND_NEQ) {
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			i += 3;
			register_condition(Cond_Type::NEQ, NeqCondition::validate, arg1, arg2);
		} else if (t.kind == Token_Kind::COND_ACTIVE) {
			std::string arg1 = tokens[i+1].text;
			i += 2;
			register_condition(Cond_Type::ACTIVE, ActiveCondition::validate, arg1);
		} else if (t.kind == Token_Kind::ACTION_BREAKWALL) {
			i += 1;
			register_action(Action_Type::BREAKWALL, BreakwallAction::act);
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

	validate_branches();
	reset_branches();
}