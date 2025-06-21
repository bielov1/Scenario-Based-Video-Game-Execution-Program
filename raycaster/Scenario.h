#pragma once

#include "Lexer.h"
#include "Event.h"
#include "Condition.h"
#include "Action.h"
#include "EventHandler.h"
#include "ConditionHandler.h"
#include "ActionHandler.h"

enum class Scenario_Level
{
	ZERO,
	EVENT,
	CONDITION,
	ACTION
};

enum class Argument_Type
{
	STRING,
	INTEGER,
	MIXED
};

struct Node
{
	int id;
	bool active;
	bool blocked;
	Scenario_Level level;
	Node* prev_node;
	Node* lact_node;
	Node* rcond_node;
	Node()
		: id(-1), active(false), blocked(false), level(Scenario_Level::ZERO), 
		  prev_node(nullptr), lact_node(nullptr), rcond_node(nullptr) {}
};

typedef Node* Scenario_Branch;
typedef Node* Branch_Node;

class Scenario
{
private:
	std::string scenario_path;
	std::string content;

	struct Validation_Error
	{
		std::string message;
		int line;
		std::string token;

		Validation_Error(const std::string& msg, int l, const std::string& t = "")
			: message(msg), line(l), token(t) 
		{}
	};

	std::vector<Scenario_Branch> branches;
	std::vector<Token> tokens;
	std::vector<Validation_Error> validation_errors;
	std::map<Token_Kind, int> command_args;
	std::map<Token_Kind, std::vector<Argument_Type>> expected_command_types;

public:
	Scenario();
	void init(std::string path);

	void get_content();

	void init_com_args();
	void init_expected_types();
	bool is_valid_number(const std::string& str);
	bool starts_with_char(const char& x);
	bool validate_argument_type(const Token& arg, int arg_pos, Argument_Type expected_type);
	bool validate_command_args(const Token& t, size_t& index);
	bool validate_structure();
	std::string get_validation_report();
	std::string form_scenario_validation_report();

	void execute_actions(Node* action_branch);
	void reset_branches();
	void validate_branches();

	void add_node_to_branch(Scenario_Branch branch, Branch_Node node);
	template<typename Func, typename... Args>
	void register_event(Event_Type type, Func func, Args... args);

	template<typename Func, typename... Args>
	void register_condition(Cond_Type type, Func func, Args... args);

	template<typename Func, typename... Args>
	void register_action(Action_Type type, Func func, Args... args);

	void parse();

	Lexer lexer;
	WorldMap world_map;
};