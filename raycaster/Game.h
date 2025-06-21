#pragma once
#include <Windows.h>
#include "Raycaster.h"
#include "Scenario.h"

enum class Game_State
{
	LAUNCH,
	INPUT_SCENARIO,
	RUN,
	EXIT
};

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
public:
	Game();
	void init_game(HWND hwnd, TCHAR* path);
	
	int on_timer();

	void ResizeDIBSection();
	void DisplayBufferToWindow(HDC hdc, RECT window_rect);
	
	void Render_Failed_Screen(HDC hdc, int width, int height);
	void Render_Victory_Screen(HDC hdc, int width, int height);
	void draw_timer_to_frame_buffer();
	void show_validation_errors(HDC hdc, int width, int height);

	void draw_text_on_screen(HDC hdc, std::string text, RECT text_rect, COLORREF text_color, UINT format, int font_size = 24);
	
	static const int FPS = 120;

	int screen_width;
	int screen_height;

	ULONGLONG timer_start_time;

	std::string validation_errors;

	Game_State state;
	Back_Buffer frame_buffer;

	HWND Hwnd;
	Scenario scenario;
	Raycaster raycaster;
};

