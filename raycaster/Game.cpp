#include "Game.h"
#include "InteractionEvent.h"
#include "KeyboardEvent.h"
#include "ObjectEvent.h"
#include "BreakwallAction.h"
#include "AnyCondition.h"
#include "NoneCondition.h"



Game::Game()
	: screen_width(0), screen_height(0), frame_buffer{}, Hwnd(0), scenario_path(""), content("")
{
}

void Game::ResizeDIBSection()
{
	if (frame_buffer.memory) {
		VirtualFree(frame_buffer.memory, 0, MEM_RELEASE);
	}

	frame_buffer.width = screen_width;
	frame_buffer.height = screen_height;
	frame_buffer.bytes_per_pixel = 4;

	frame_buffer.info.bmiHeader.biSize = sizeof(frame_buffer.info.bmiHeader);
	frame_buffer.info.bmiHeader.biWidth = frame_buffer.width;
	frame_buffer.info.bmiHeader.biHeight = -frame_buffer.height;
	frame_buffer.info.bmiHeader.biPlanes = 1;
	frame_buffer.info.bmiHeader.biBitCount = 32;
	frame_buffer.info.bmiHeader.biCompression = BI_RGB;

	int bitmap_memory_size = (frame_buffer.width*frame_buffer.height)*frame_buffer.bytes_per_pixel;
	frame_buffer.memory = VirtualAlloc(0, bitmap_memory_size, MEM_COMMIT, PAGE_READWRITE);
}

void Game::DisplayBufferToWindow(HDC hdc, RECT window_rect)
{
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;
	StretchDIBits(hdc, 0, 0, frame_buffer.width, frame_buffer.height, 
		0, 0, window_width, window_height,
		frame_buffer.memory, &frame_buffer.info,
		DIB_RGB_COLORS, SRCCOPY);
}

void Game::init_game(HWND hwnd, std::string path)
{
	SetTimer(hwnd, WM_USER + 1, 1000 / FPS, 0);
	Hwnd = hwnd;
	scenario_path = path;
	RECT ClientRect;
	GetClientRect(hwnd, &ClientRect);
	screen_width = ClientRect.right - ClientRect.left;
	screen_height = ClientRect.bottom - ClientRect.top;
	raycaster.init();
	get_content();

	lexer.init(content, static_cast<std::size_t>(content.length()));
	Token t;
	for(;;) {
		t = lexer.next_token();
		if (t.kind == Token_Kind::END) break;
		tokens.push_back(t);
	}

	parse();
}

int Game::on_timer()
{
	InvalidateRect(Hwnd, NULL, FALSE);
	return 0;
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

void Game::execute_actions(Node* action_branch)
{
	Node* act_node = action_branch;
	while (act_node != nullptr) {
		ExecuteAction(act_node->id);
		act_node = act_node->lact_node;
	}
}

void Game::reset_branches()
{
	for (const Scenario_Branch branch : scenario) {
		Node* node = branch;
		while (node != nullptr) {
			node->active = false;
			node = node->rcond_node;
		}
	}
}

void Game::validate_branches()
{
	std::stack<Node*> stack;
	for (const Scenario_Branch event : scenario) {
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

void Game::add_node_to_branch(Scenario_Branch branch, Branch_Node node)
{
	if (branch == nullptr || node == nullptr)
		return;

	Node* next = branch;
	if (node->level == Scenario_Level::CONDITION) {
		for (;next->rcond_node != nullptr; next = next->rcond_node);
		next->rcond_node = node;
	} else if (node->level == Scenario_Level::ACTION) {
		for (;next->rcond_node != nullptr; next = next->rcond_node);
		for (;next->lact_node != nullptr; next = next->lact_node);
		next->lact_node = node;
	}
}

void Game::parse()
{
	std::size_t i = 0;
	while (i < tokens.size())
	{
		if (tokens[i].kind == Token_Kind::EVENT_INTERACTION) {
			std::string arg1 = tokens[i+1].text;
			std::string arg2 = tokens[i+2].text;
			i += 3;
			Scenario_Branch interaction_branch = new Node();
			int interaction_listener_id = RegisterEvent(this, interaction_branch, Event_Type::INTERACTION, InteractionEvent::check, arg1, arg2);
			interaction_branch->id = interaction_listener_id;
			interaction_branch->active = false;
			interaction_branch->level = Scenario_Level::EVENT;
			scenario.push_back(interaction_branch);
		} else if (tokens[i].kind == Token_Kind::EVENT_OBJECT) {
			std::string arg = tokens[i+1].text;
			i += 2;
			//Scenario_Branch object_branch;
			//AddEventListener(Event_Type::OBJECT, ObjectEvent::check, this, &object_branch.node, arg);
		} else if (tokens[i].kind == Token_Kind::EVENT_KEYBOARD) {
			//Event_Type type;
			const char* arg = tokens[i+1].text.c_str();

			/*if (std::strcmp(arg, "UP") == 0) {
				type = Event_Type::KEYBOARD_UP;
				AddEventListener(this, type, KeyboardUPEvent::check, arg);
			} else if (std::strcmp(arg, "DOWN") == 0) {
				type = Event_Type::KEYBOARD_DOWN;
				AddEventListener(this, type, KeyboardDOWNEvent::check, arg);
			} else if (std::strcmp(arg, "LEFT") == 0) {
				type = Event_Type::KEYBOARD_LEFT;
				AddEventListener(this, type, KeyboardLEFTEvent::check, arg);
			} else if (std::strcmp(arg, "RIGHT") == 0) {
				type = Event_Type::KEYBOARD_RIGHT;
				AddEventListener(this, type, KeyboardRIGHTEvent::check, arg);
			} else {
				std::cerr << "ERROR: UNKNOWN EVENT_KEYBOARD: " << arg;
				exit(1);
			}*/

			i += 2;
		} else if (tokens[i].kind == Token_Kind::COND_ANY) {
			Scenario_Branch last_parsed_branch = scenario.back();
			Branch_Node any_node = new Node();
			int id = RegisterCondition(this, any_node, Cond_Type::ANY, AnyCondition::validate);
			any_node->id = id;
			any_node->active = false;
			any_node->level = Scenario_Level::CONDITION;
			add_node_to_branch(last_parsed_branch, any_node);
			++i;
		} else if (tokens[i].kind == Token_Kind::COND_NONE) {
			Scenario_Branch last_parsed_branch = scenario.back();
			Branch_Node none_node = new Node();
			int id = RegisterCondition(this, none_node, Cond_Type::NONE, NoneCondition::validate);
			none_node->id = id;
			none_node->active = false;
			none_node->level = Scenario_Level::CONDITION;
			add_node_to_branch(last_parsed_branch, none_node);
			++i;
		} else if (tokens[i].kind == Token_Kind::ACTION_BREAKWALL) {
			Scenario_Branch last_parsed_branch = scenario.back();
			std::string arg = tokens[i+1].text;
			int id = RegisterAction(this, Action_Type::BREAKWALL, BreakwallAction::act, arg);
			Branch_Node breakwall_node = new Node(id, true, Scenario_Level::ACTION);
			add_node_to_branch(last_parsed_branch, breakwall_node);
			i += 2;

		} else {
			std::cerr << "uknown kind of token\n.";
			exit(1);
		}

	}
}