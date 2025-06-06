#include "Game.h"


Game::Game()
	: screen_width(0), screen_height(0),
	  frame_buffer{}, state(Game_State::LAUNCH), Hwnd(0)
{
}

void Game::init_game(HWND hwnd, std::string path)
{
	SetTimer(hwnd, WM_USER + 1, 1000 / FPS, 0);
	SetTimer(hwnd, WM_USER + 2, 1000, 0);
	state = Game_State::RUN;
	Hwnd = hwnd;
	scenario.init(path);
	RECT ClientRect;
	GetClientRect(hwnd, &ClientRect);
	screen_width = ClientRect.right - ClientRect.left;
	screen_height = ClientRect.bottom - ClientRect.top;

	raycaster.init(scenario.world_map.player, scenario.world_map.map_width, scenario.world_map.map_width);

}

int Game::on_timer()
{
	if (state == Game_State::RUN) {
		scenario.validate_branches();
		scenario.reset_branches();
	}
	return 0;
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

void Game::draw_text_on_screen(HDC hdc, std::string text, RECT text_rect, COLORREF text_color, UINT format)
{
	SetTextColor(hdc, text_color);
	SetBkMode(hdc, TRANSPARENT);

	DrawTextA(hdc, text.c_str(), -1, &text_rect, format);

}