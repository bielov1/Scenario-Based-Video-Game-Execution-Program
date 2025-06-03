#pragma once
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
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

enum class Scenario_Level
{
	ZERO,
	EVENT,
	CONDITION,
	ACTION
};

enum class Game_State
{
	START,
	PLAYING,
	FAILED,
	VICTORY
};

struct Back_Buffer
{
	BITMAPINFO info;
	void* memory;
	int width;
	int height;
	int bytes_per_pixel;
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
	void Render_Failed_Screen(HDC hdc, int width, int height);

	void init_game(HWND hwnd, std::string path);
	int on_timer();
	void draw_text_on_screen(HDC hdc, std::string text, RECT text_rect, COLORREF text_color, UINT format);
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
	WorldMap& worldmapInstance();

	std::vector<Scenario_Branch> scenario;

	static const int FPS = 60;

	int screen_width;
	int screen_height;
	int uptime_in_secs;

	Game_State state;
	Back_Buffer frame_buffer;

	HWND Hwnd;
	Lexer lexer;
	Raycaster raycaster;
	WorldMap world_map;
};

