#include "Game.h"
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




Game::Game()
	: screen_width(0), screen_height(0), uptime_in_secs(0), 
	  frame_buffer{}, state(Game_State::START), Hwnd(0), 
	  scenario_path(""), content("")
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

void Game::Render_Failed_Screen(HDC hdc, int width, int height)
{
	if (!frame_buffer.memory) return;

	uint32_t red_pixel = 0x00FF0000;

	uint32_t* buffer = reinterpret_cast<uint32_t*>(frame_buffer.memory);
	int total_pixels = frame_buffer.width * frame_buffer.height;

	for (int i = 0; i < total_pixels; ++i) {
		buffer[i] = red_pixel;
	}

	RECT window_rect = { 0, 0, width, height };
	DisplayBufferToWindow(hdc, window_rect);
}

void Game::Render_Victory_Screen(HDC hdc, int width, int height)
{
	if (!frame_buffer.memory) return;

	uint32_t red_pixel = 0x0000FF00;

	uint32_t* buffer = reinterpret_cast<uint32_t*>(frame_buffer.memory);
	int total_pixels = frame_buffer.width * frame_buffer.height;

	for (int i = 0; i < total_pixels; ++i) {
		buffer[i] = red_pixel;
	}

	RECT window_rect = { 0, 0, width, height };
	DisplayBufferToWindow(hdc, window_rect);
}

void Game::init_game(HWND hwnd, std::string path)
{
	SetTimer(hwnd, WM_USER + 1, 1000 / FPS, 0);
	SetTimer(hwnd, WM_USER + 2, 1000, 0);
	state = Game_State::PLAYING;
	Hwnd = hwnd;
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

	RECT ClientRect;
	GetClientRect(hwnd, &ClientRect);
	screen_width = ClientRect.right - ClientRect.left;
	screen_height = ClientRect.bottom - ClientRect.top;

	raycaster.init(world_map.player, world_map.map_width, world_map.map_width);

}

int Game::on_timer()
{
	validate_branches();
	reset_branches();
	return 0;
}

void Game::draw_text_on_screen(HDC hdc, std::string text, RECT text_rect, COLORREF text_color, UINT format)
{
	SetTextColor(hdc, text_color);
	SetBkMode(hdc, TRANSPARENT);

	DrawTextA(hdc, text.c_str(), -1, &text_rect, format);

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
		next->rcond_node->prev_node = next;
	} else if (node->level == Scenario_Level::ACTION) {
		for (;next->rcond_node != nullptr; next = next->rcond_node);
		for (;next->lact_node != nullptr; next = next->lact_node);
		next->lact_node = node;
		next->lact_node->prev_node = next;
	}
}

template<typename Func, typename... Args>
void Game::register_event(Event_Type type, Func func, Args... args)
{
	std::vector<std::string> raw_args = { args... };
	Scenario_Branch event_branch = new Node();
	int event_id = RegisterEvent(this, event_branch, type, func, raw_args, args...);
	event_branch->id = event_id;
	event_branch->active = false;
	event_branch->level = Scenario_Level::EVENT;
	scenario.push_back(event_branch);
}

template<typename Func, typename... Args>
void Game::register_condition(Cond_Type type, Func func, Args... args)
{
	Scenario_Branch last_parsed_branch = scenario.back();
	Branch_Node cond_node = new Node();
	int condition_id = RegisterCondition(this, cond_node, type, func, args...);
	cond_node->id = condition_id;
	cond_node->active = false;
	cond_node->level = Scenario_Level::CONDITION;
	add_node_to_branch(last_parsed_branch, cond_node);
}

template<typename Func, typename... Args>
void Game::register_action(Action_Type type, Func func, Args... args)
{
	Scenario_Branch last_parsed_branch = scenario.back();
	Branch_Node act_node = new Node();
	int act_id = RegisterAction(this, act_node, type, func, args...);
	act_node->id = act_id;
	act_node->active = true;
	act_node->level = Scenario_Level::ACTION;
	add_node_to_branch(last_parsed_branch, act_node);
}

void Game::parse()
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

WorldMap& Game::worldmapInstance()
{ return world_map; }