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
	Scenario_Level level;
	Node* lact_node;
	Node* rcond_node;
	Node()
		: id(-1), active(false), level(Scenario_Level::ZERO), lact_node(nullptr), rcond_node(nullptr) {}
	Node(int id, bool active, Scenario_Level l)
		: id(id), active(active), level(l), lact_node(nullptr), rcond_node(nullptr) {}
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

	void init_game(HWND hwnd, std::string path);
	int on_timer();
	void get_content();
	void execute_actions(Node* action_branch);
	void reset_branches();
	void validate_branches();
	void add_node_to_branch(Scenario_Branch branch, Branch_Node node);
	void parse();

	std::vector<Scenario_Branch> scenario;

	static const int FPS = 60;

	int screen_width;
	int screen_height;

	Back_Buffer frame_buffer;

	HWND Hwnd;
	Lexer lexer;
	Raycaster raycaster;


};

