#pragma once
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include "Raycaster.h"
#include "Lexer.h"
#include "Event.h"
#include "Condition.h"
#include "Action.h"
#include "EventHandler.h"
#include "ConditionHandler.h"
#include "ActionHandler.h"
#include "Vector2.h"


class InteractionEvent;
class KeyboardUPEvent;
class KeyboardDOWNEvent;
class KeyboardLEFTEvent;
class KeyboardRIGHTEvent;
class ObjectEvent;
class BreakwallAction;

//-------------------------------------------------
class Game
{
private:
	std::string scenario_path;
	std::string content;

	std::vector<Token> tokens;

public:
	Game();

	void init_game(HWND hwnd, std::string path);
	void get_content();
	bool verify_all_conditions_in_map(Event_Type e);
	void clear_statuses();
	void check_events();
	void parse();

	std::vector<Event_Type> event_container;
	std::vector<Cond_Type> condition_container;
	std::vector<Action_Type> action_container;

	std::map<Event_Type, std::vector<Cond_Type>> event_condition_map;
	std::map<Event_Type, std::vector<Action_Type>> event_action_map;

	std::map<Event_Type, bool> events_status;
	std::map<Event_Type, std::vector<bool>> conditions_status_for_event_type;

	HWND Hwnd;
	Lexer lexer;
	Raycaster raycaster;
};

