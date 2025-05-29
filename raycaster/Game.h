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


class InteractionEvent;
class KeyboardUPEvent;
class KeyboardDOWNEvent;
class KeyboardLEFTEvent;
class KeyboardRIGHTEvent;
class ObjectEvent;
class BreakwallAction;

struct Back_Buffer
{
	BITMAPINFO info;
	void* memory;
	int width;
	int height;
	int bytes_per_pixel;
};


//-------------------------------------------------
class Game
{
private:
	std::string scenario_path;
	std::string content;

	std::vector<Token> tokens;

public:
	Game();

	void ResizeDIBSection();
	void DisplayBufferToWindow(HDC hdc, RECT window_rect);

	void init_game(HWND hwnd, std::string path);
	int on_timer();
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

	static const int FPS = 60;

	int screen_width;
	int screen_height;

	Back_Buffer frame_buffer;

	HWND Hwnd;
	Lexer lexer;
	Raycaster raycaster;


};

