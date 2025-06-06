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
		: id(-1), active(false), blocked(false), level(Scenario_Level::ZERO), prev_node(nullptr), lact_node(nullptr), rcond_node(nullptr) {}
};

typedef Node* Scenario_Branch;
typedef Node* Branch_Node;

class Scenario
{
private:
	std::string scenario_path;
	std::string content;

	std::vector<Token> tokens;
public:
	Scenario();
	void init(std::string path);

	void get_content();
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
	std::vector<Scenario_Branch> branches;

	Lexer lexer;
	WorldMap world_map;
};