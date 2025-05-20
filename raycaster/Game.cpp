#include "Game.h"

#include "InteractionEvent.h"
#include "KeyboardEvent.h"
#include "ObjectEvent.h"
#include "BreakwallAction.h"
#include "AnyCondition.h"

std::string token_kind_name(Token_Kind kind)
{
	switch(kind) {
	case Token_Kind::END:
		return "end of content";
	case Token_Kind::UNKNOWN:
		return "unknown";
	case Token_Kind::EVENT_INTERACTION:
		return "interaction event";
	case Token_Kind::EVENT_OBJECT:
		return "object event";
	case Token_Kind::EVENT_KEYBOARD:
		return "keyboard event";
	case Token_Kind::COND_ANY:
		return "any condition";
	case Token_Kind::ACTION_SUB:
		return "sub action";
	case Token_Kind::ACTION_BREAKWALL:
		return "breakwall action";
	case Token_Kind::ARGUMENT:
		return "argument";
	case Token_Kind::ACCESS_PROP:
		return "access property";
	default:
		fprintf(stderr, "Error: token_kind_name()\n");
		exit(1);
	}
	return nullptr;
}

Game::Game(std::string path)
	: scenario_path(path), content("")
{
	init();
}

void Game::init()
{
	player_.init(100, 5, "");
	wall_.init(5);

	get_content();

	lexer.init(content, static_cast<std::size_t>(content.length()));
	Token t;
	for(;;) {
		t = lexer.next_token();
		if (t.kind == Token_Kind::END) break;
		std::cout << t.text << " (" << token_kind_name(t.kind) << ") " << std::endl; 
		tokens.push_back(t);
	}

	parse();
	check_events();
	clear_statuses();
}

void Game::get_content()
{
	std::ifstream scen(scenario_path);
	std::string line;
	while (std::getline(scen, line)) {
		if (line.empty() || (line[0] == '/' && line[1] == '/')) continue;
		content += line;
		content.push_back('\n');
	}
}

bool Game::verify_all_conditions_in_map(Event_Type e)
{
	auto it = conditions_status_for_event_type.find(e);
	if (it != conditions_status_for_event_type.end()) {
		for (const auto& valid : it->second) {
			if (!valid) {
				return false;
			}
		}
	}

	return true;
}

void Game::clear_statuses()
{
	//events_status;
	//conditions_status_for_event_type;

	for (auto& pair : conditions_status_for_event_type) {
		pair.second.clear();
	}


}

void Game::check_events()
{
	for (const auto& e_type : event_container)
	{
		DispatchEvent(e_type);
		bool event_type_is_active = events_status[e_type];
		if (event_type_is_active) {
			auto it = event_condition_map.find(e_type);
			if (it != event_condition_map.end()) {
				for (const auto& x : it->second) {
					ValidateCondition(x);
				} 

				if (verify_all_conditions_in_map(e_type)) {
					auto it = event_action_map.find(e_type);
					if (it != event_action_map.end()) {
						for (auto& a_type : it->second) {
							ExecuteAction(a_type);
						}
					}
				}
			}
		}
	}
}

void Game::parse()
{
	std::size_t i = 0;
	while (i < tokens.size())
	{
		if (tokens[i].kind == Token_Kind::EVENT_INTERACTION) {
			Event_Type type = Event_Type::INTERACTION;
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			i += 3;
			events_status.insert(std::make_pair(type, false));
			event_container.push_back(type);
			AddInteractionEventListener(this, type, InteractionEvent::check, arg1, arg2);
		} else if (tokens[i].kind == Token_Kind::EVENT_OBJECT) {
			Event_Type type = Event_Type::OBJECT;
			std::string arg = tokens[i+1].text;
			i += 2;
			events_status.insert(std::make_pair(type, false));
			event_container.push_back(type);
			AddObjectEventListener(this, type, ObjectEvent::check, arg);
		} else if (tokens[i].kind == Token_Kind::EVENT_KEYBOARD) {
			Event_Type type;
			const char* arg = tokens[i+1].text.c_str();

			if (std::strcmp(arg, "UP") == 0) {
				type = Event_Type::KEYBOARD_UP;
				AddKeyboardEventListener(this, type, KeyboardUPEvent::check, arg);
			} else if (std::strcmp(arg, "DOWN") == 0) {
				type = Event_Type::KEYBOARD_DOWN;
				AddKeyboardEventListener(this, type, KeyboardDOWNEvent::check, arg);
			} else if (std::strcmp(arg, "LEFT") == 0) {
				type = Event_Type::KEYBOARD_LEFT;
				AddKeyboardEventListener(this, type, KeyboardLEFTEvent::check, arg);
			} else if (std::strcmp(arg, "RIGHT") == 0) {
				type = Event_Type::KEYBOARD_RIGHT;
				AddKeyboardEventListener(this, type, KeyboardRIGHTEvent::check, arg);
			} else {
				std::cerr << "ERROR: UNKNOWN EVENT_KEYBOARD: " << arg;
				exit(1);
			}

			i += 2;
			events_status.insert(std::make_pair(type, false));
			event_container.push_back(type);
		} else if (tokens[i].kind == Token_Kind::COND_ANY) {
			Event_Type last_parsed_event = event_container.back();
			condition_container.push_back(Cond_Type::ANY);
			event_condition_map[last_parsed_event].push_back(Cond_Type::ANY);
			RegisterAnyConditionType(this, Cond_Type::ANY, AnyCondition::validate, last_parsed_event);
			++i;
		} else if (tokens[i].kind == Token_Kind::ACTION_BREAKWALL) {
			Event_Type last_parsed_event = event_container.back();
			action_container.push_back(Action_Type::BREAKWALL);
			event_action_map[last_parsed_event].push_back(Action_Type::BREAKWALL);
			RegisterBreakwallActionType(this, Action_Type::BREAKWALL, BreakwallAction::act);
			++i;

		} else {
			std::cerr << "uknown kind of token\n.";
			exit(1);
		}

	}
}

Player& Game::playerInstance()
{ return player_;}

Wall& Game::wallInstance()
{ return wall_; }