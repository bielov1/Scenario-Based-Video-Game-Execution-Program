// raycaster.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING  100

// Global Variables:
Game game;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_RAYCASTER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYCASTER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYCASTER));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = CreateSolidBrush(RGB(18, 18, 18));
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RAYCASTER);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == 0)
	{
		return FALSE;
	}

	game.init_game(hWnd, "game.txt");
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
	{
		RECT ClientRect;
		GetClientRect(hWnd, &ClientRect);
		game.screen_width = ClientRect.right - ClientRect.left;
		game.screen_height = ClientRect.bottom - ClientRect.top;
		game.ResizeDIBSection();
	}
	break;
	case WM_KEYDOWN:
	{
		if (wParam == 'S')
			game.world_map.player.pos = game.world_map.player.pos.sub(from_angle(game.world_map.player.dir).mul(game.world_map.player.move_speed));
		else if (wParam == 'W')
			game.world_map.player.pos = game.world_map.player.pos.add(from_angle(game.world_map.player.dir).mul(game.world_map.player.move_speed));
		else if (wParam == VK_LEFT)
			game.world_map.player.dir -= game.world_map.player.rotation_speed;
		else if (wParam == VK_RIGHT)
			game.world_map.player.dir += game.world_map.player.rotation_speed;
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		RECT text_rect;
		RECT ClientRect;
		GetClientRect(hWnd, &ClientRect);
		int screen_width = ClientRect.right - ClientRect.left;
		int screen_height = ClientRect.bottom - ClientRect.top;

		if (game.state == Game_State::PLAYING) {
			if (screen_width <= 0 || screen_height <= 0) {
				EndPaint(hWnd, &ps);
				break;
			}
		
			void* render_result = game.raycaster.render_frame(game.world_map, game.world_map.player, screen_width, screen_height);
			if (render_result)
			{
				memcpy(game.frame_buffer.memory, render_result, screen_width * screen_height * sizeof(Pixel));
				delete[] static_cast<Pixel*>(render_result);
			}

			game.DisplayBufferToWindow(hdc, ClientRect);

			if (game.world_map.quest_timer.is_active()) {
				std::string timer_count_str = std::to_string(game.world_map.quest_timer.get_timer_count());
				text_rect.left = 10;
				text_rect.top = 10;
				text_rect.bottom = 50;
				text_rect.right = 100;
				game.draw_text_on_screen(hdc, timer_count_str, text_rect, RGB(255, 255, 255), DT_LEFT | DT_TOP | DT_SINGLELINE);
			}
		} else if (game.state == Game_State::FAILED) {
			game.Render_Failed_Screen(hdc, screen_width, screen_height);
			int text_size = 100;
			text_rect.left   = screen_width  / 2 - text_size / 2;
			text_rect.top    = screen_height / 2 - text_size / 2;
			text_rect.right  = screen_width  / 2 + text_size / 2;
			text_rect.bottom = screen_height / 2 + text_size / 2;
			game.draw_text_on_screen(hdc, "FAILED", text_rect, RGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		} else if (game.state == Game_State::VICTORY) {
			game.Render_Victory_Screen(hdc, screen_width, screen_height);
			int text_size = 100;
			text_rect.left   = screen_width  / 2 - text_size / 2;
			text_rect.top    = screen_height / 2 - text_size / 2;
			text_rect.right  = screen_width  / 2 + text_size / 2;
			text_rect.bottom = screen_height / 2 + text_size / 2;
			game.draw_text_on_screen(hdc, "VICTORY", text_rect, RGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		EndPaint(hWnd, &ps);

		
	}
	break;
	case WM_TIMER:
		if (wParam == WM_USER + 1) {
			InvalidateRect(hWnd, NULL, FALSE);
			return game.on_timer();
		} else if (wParam == WM_USER + 2) {
			if (game.world_map.quest_timer.is_active()) {
				game.uptime_in_secs++;
			}
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, WM_USER + 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}