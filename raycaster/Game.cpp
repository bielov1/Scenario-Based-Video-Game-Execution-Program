#include "Game.h"


Game::Game()
	: screen_width(0), screen_height(0), timer_start_time(0),
	  frame_buffer{}, state(Game_State::LAUNCH), Hwnd(0)
{
}

void Game::init_game(HWND hwnd, TCHAR* path)
{
	std::string path_str;

#ifdef UNICODE
	int size = WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr);
	if (size > 0) {
		std::string result(size-1, '\0');
		WideCharToMultiByte(CP_UTF8, 0, path, -1, &result[0], size, nullptr, nullptr);
		path_str = result;
	}
#else
	path_str = path;
#endif

	scenario.init(path_str);
	validation_errors = scenario.form_scenario_validation_report();
	if (validation_errors.empty()) {
		DragAcceptFiles(hwnd, FALSE);
		Hwnd = hwnd;
		state = Game_State::RUN;
		SetTimer(hwnd, WM_USER + 1, 1000 / FPS, 0);
		scenario.parse();
		RECT ClientRect;
		GetClientRect(hwnd, &ClientRect);
		screen_width = ClientRect.right - ClientRect.left;
		screen_height = ClientRect.bottom - ClientRect.top;

		raycaster.init(scenario.world_map.player, scenario.world_map.map_width, scenario.world_map.map_width);
	} else {
		InvalidateRect(hwnd, NULL, FALSE);
	}
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

	uint32_t red_pixel = RGB(0, 0, 255);

	uint32_t* buffer = reinterpret_cast<uint32_t*>(frame_buffer.memory);
	int total_pixels = frame_buffer.width * frame_buffer.height;

	for (int i = 0; i < total_pixels; ++i) {
		buffer[i] = red_pixel;
	}

	RECT window_rect = { 0, 0, width, height };
	DisplayBufferToWindow(hdc, window_rect);

	RECT text_rect{};
	int textframe_size = 200;
	text_rect.left   = screen_width  / 2 - textframe_size / 2;
	text_rect.top    = screen_height / 2 - textframe_size / 2;
	text_rect.right  = screen_width  / 2 + textframe_size / 2;
	text_rect.bottom = screen_height / 2 + textframe_size / 2;
	draw_text_on_screen(hdc, "FAILED", text_rect, RGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_SINGLELINE, 32);
}

void Game::Render_Victory_Screen(HDC hdc, int width, int height)
{
	if (!frame_buffer.memory) return;

	uint32_t green_pixel = RGB(0, 255, 0);

	uint32_t* buffer = reinterpret_cast<uint32_t*>(frame_buffer.memory);
	int total_pixels = frame_buffer.width * frame_buffer.height;

	for (int i = 0; i < total_pixels; ++i) {
		buffer[i] = green_pixel;
	}

	RECT window_rect = { 0, 0, width, height };
	DisplayBufferToWindow(hdc, window_rect);

	RECT text_rect{};
	int textframe_size = 200;
	text_rect.left   = screen_width  / 2 - textframe_size / 2;
	text_rect.top    = screen_height / 2 - textframe_size / 2;
	text_rect.right  = screen_width  / 2 + textframe_size / 2;
	text_rect.bottom = screen_height / 2 + textframe_size / 2;
	draw_text_on_screen(hdc, "VICTORY", text_rect, RGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_SINGLELINE, 32);
}

void Game::draw_timer_to_frame_buffer()
{
	if (!scenario.world_map.quest_timer.is_active() || !frame_buffer.memory) 
		return;

	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);

	int timerWidth = 55;
	int timerHeight = 50;

	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, timerWidth, timerHeight);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

	RECT bgRect = {0, 0, timerWidth, timerHeight};
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
	FillRect(hdcMem, &bgRect, hBrush);
	DeleteObject(hBrush);

	std::string timer_text = std::to_string(scenario.world_map.quest_timer.get_timer_count());

	HFONT hFont = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
	HFONT hOldFont = (HFONT)SelectObject(hdcMem, hFont);

	SetTextColor(hdcMem, RGB(255, 255, 255));
	SetBkMode(hdcMem, TRANSPARENT);

	RECT textRect = {5, 5, timerWidth - 5, timerHeight - 5};
	DrawTextA(hdcMem, timer_text.c_str(), -1, &textRect, DT_LEFT | DT_TOP);

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = timerWidth;
	bmi.bmiHeader.biHeight = -timerHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	uint32_t* timerPixels = new uint32_t[timerWidth * timerHeight];
	GetDIBits(hdcMem, hBitmap, 0, timerHeight, timerPixels, &bmi, DIB_RGB_COLORS);

	uint32_t* framePixels = (uint32_t*)frame_buffer.memory;
	int startX = 10;
	int startY = 10;

	uint32_t transparent_сolor = RGB(255, 0, 255) & 0x00FFFFFF;

	for (int y = 0; y < timerHeight; y++) {
		for (int x = 0; x < timerWidth; x++) {
			int frameX = startX + x;
			int frameY = startY + y;

			if (frameX >= 0 && frameX < frame_buffer.width && 
				frameY >= 0 && frameY < frame_buffer.height) {

				uint32_t timerPixel = timerPixels[y * timerWidth + x];
				uint32_t pixelColor = timerPixel & 0x00FFFFFF;

				if (pixelColor != transparent_сolor) {
					framePixels[frameY * frame_buffer.width + frameX] = timerPixel;
				}
			}
		}
	}

	delete[] timerPixels;
	SelectObject(hdcMem, hOldFont);
	DeleteObject(hFont);
	SelectObject(hdcMem, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);
}

void Game::show_validation_errors(HDC hdc, int width, int height)
{
	if (!frame_buffer.memory) return;

	uint32_t dark_bg = RGB(18, 18, 18);
	uint32_t* buffer = reinterpret_cast<uint32_t*>(frame_buffer.memory);
	int total_pixels = frame_buffer.width * frame_buffer.height;

	for (int i = 0; i < total_pixels; ++i) {
		buffer[i] = dark_bg;
	}

	RECT window_rect = {0, 0, width, height};
	DisplayBufferToWindow(hdc, window_rect);

	RECT title_rect = {20, 20, width - 20, 60};
	draw_text_on_screen(hdc, "Scenario Validation Errors", title_rect, RGB(255, 0, 0), DT_LEFT | DT_TOP, 28);

	RECT error_rect = {20, 80, width - 20, height - 100};
	draw_text_on_screen(hdc, validation_errors, error_rect, RGB(255, 0, 0), DT_LEFT | DT_TOP | DT_WORDBREAK, 18);

	RECT instruction_rect = {20, height - 80, width - 20, height - 20};
	draw_text_on_screen(hdc, "Drag and drop a valid scenario file to continue...", instruction_rect, RGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_SINGLELINE, 16);
}

void Game::draw_text_on_screen(HDC hdc, std::string text, RECT text_rect, COLORREF text_color, UINT format, int font_size)
{
	HFONT font = CreateFont(
		font_size,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Arial"
	);

	HFONT old_font = (HFONT)SelectObject(hdc, font);

	SetTextColor(hdc, text_color);
	SetBkMode(hdc, TRANSPARENT);
	DrawTextA(hdc, text.c_str(), -1, &text_rect, format);

	SelectObject(hdc, old_font);
	DeleteObject(font);
}