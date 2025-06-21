// raycaster.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING  100
#define IDC_CREATE_SCENARIO_BUTTON WM_USER + 1000

// Global Variables:
Game game;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hCreateScenarioButton;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void on_create(HWND hWnd, WPARAM wParam, LPARAM lParam);
void on_command(HWND hWnd, WPARAM wParam, LPARAM lParam);
void on_keydown(HWND hWnd, WPARAM wParam, LPARAM lParam);
void on_drawitem(HWND hWnd, WPARAM wParam, LPARAM lParam);
void on_paint(HWND hWnd, WPARAM wParam, LPARAM lParam);

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

	HWND hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == 0)
	{
		return FALSE;
	}
	DragAcceptFiles(hWnd, TRUE);
	game.state = Game_State::INPUT_SCENARIO;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void HandleFiles(HWND hwnd, WPARAM wParam)
{
	TCHAR szName[MAX_PATH];
	HDROP hDrop = (HDROP)wParam;
	int count = DragQueryFile(hDrop, 0xFFFFFFFF, szName, 200);
	if (count > 0) {
		DragQueryFile(hDrop, 0, szName, MAX_PATH);
		game.init_game(hwnd, szName);
	}
	DragFinish(hDrop);
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
	case WM_DROPFILES:
	{
		HandleFiles(hWnd, wParam);
	}
	break;
	case WM_SIZE:
	{
		RECT ClientRect;
		GetClientRect(hWnd, &ClientRect);
		game.screen_width = ClientRect.right - ClientRect.left;
		game.screen_height = ClientRect.bottom - ClientRect.top;
		game.ResizeDIBSection();

		if (hCreateScenarioButton != NULL) {
			SetWindowPos(hCreateScenarioButton, 
				NULL,
				game.screen_width - 160,
				10,
				0, 0,
				SWP_NOZORDER | SWP_NOSIZE);
		}
	}
	break;
	case WM_CREATE:
	{
		on_create(hWnd, wParam, lParam);
	}
	break;
	case WM_KEYDOWN:
	{
		on_keydown(hWnd, wParam, lParam);
	}
	break;
	case WM_DRAWITEM:
	{
		on_drawitem(hWnd, wParam, lParam);
		return TRUE;
	}
	break;
	case WM_COMMAND:
	{
		on_command(hWnd, wParam, lParam);
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
		on_paint(hWnd, wParam, lParam);
	}
	break;
	case WM_TIMER:
		if (wParam == WM_USER + 1) {
			game.on_timer();
			if (game.scenario.world_map.quest_timer.is_active()) {
				ULONGLONG current_time = GetTickCount64();

				if (game.timer_start_time == 0) {
					game.timer_start_time = current_time;
				}

				if (current_time - game.timer_start_time >= 1000) {
					game.scenario.world_map.uptime_in_secs++;
					game.timer_start_time = current_time;
				}
			} else {
				game.timer_start_time = 0;
			}

			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case WM_DESTROY:
		DestroyWindow(hCreateScenarioButton);
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

void on_create(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int button_width = 150;
	int button_height = 40;
	RECT ClientRect;
	GetClientRect(hWnd, &ClientRect);
	int screen_width = ClientRect.right - ClientRect.left;
	int screen_height = ClientRect.bottom - ClientRect.top;
	hCreateScenarioButton = CreateWindowEx(0,
										   L"Button",
										   L"Create scenario file",
										   WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
										   screen_width - button_width - 10,
										   10,
										   button_width,
										   button_height,
										   hWnd,
										   (HMENU)(IDC_CREATE_SCENARIO_BUTTON),
										   (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
										   NULL);


}

void on_command(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDC_CREATE_SCENARIO_BUTTON) {
		WCHAR current_dir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, current_dir);

		WCHAR scenarios_path[MAX_PATH];
		wsprintf(scenarios_path, L"%s\\Scenarios", current_dir);

		DWORD dwAttrib = GetFileAttributes(scenarios_path);
		if (dwAttrib == INVALID_FILE_ATTRIBUTES || !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
			if (!CreateDirectory(scenarios_path, NULL)) {
				MessageBox(hWnd, L"Failed to create Scenarios directory", L"Error", MB_OK);
				return;
			}
		}

		int index = 0;
		WCHAR file_path[MAX_PATH];
		WCHAR file_name[MAX_PATH];

		while (true) {
			wsprintf(file_name, L"scenario_%02d.txt", index);
			wsprintf(file_path, L"%s\\%s", scenarios_path, file_name);

			DWORD dwAttrib = GetFileAttributes(file_path);
			if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
				break;
			}

			index++;

			if (index > 99) {
				MessageBox(hWnd, L"Too many scenario files!", L"Error", MB_OK | MB_ICONERROR);
				return;
			}
		}

		HANDLE hFile = CreateFile(file_path,
								  GENERIC_WRITE,
								  0,
								  NULL,
								  CREATE_NEW,
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);

		if (hFile == INVALID_HANDLE_VALUE) {
			MessageBox(hWnd, L"Failed to create scenario file.", L"Error", MB_OK | MB_ICONERROR);
			return;
		}

		const char* hint_template = "// NOTATION\n//\n"
									"// events description:\n"
									"// map - initializes the game location. This command sets up all game objects and their properties and is executed only once per game.\n"
									"// interaction - manages interaction between two game objects. This event triggers whe two objects occupy the same position on map.\n"
									"// timer - triggers automatically at a specified time interval.\n//\n"
									"// conditions description:\n"
									"// active - checks if a game object is active.\n"
									"// eq - checks for equality between a game object property and given value.\n"
									"// neq - checks for inequality between a game object property and given value.\n"
									"// none - check is always inactive.\n"
									"// any - check is always active.\n//\n"
									"// actions description:\n"
									"// breakwall - breaks game object wall.\n"
									"// build - creates a new wall with specified information.\n"
									"// failed - shows failed screen.\n"
									"// victory - shows victory screen.\n"
									"// remove - removes a game object from location.\n"
									"// set - sets a specified value to game object property.\n"
									"// showall - shows all same objects on location.\n"
									"// sub - substructes a specified value from a game object property\n\n";

		WriteFile(hFile, hint_template, strlen(hint_template), NULL, NULL);

		CloseHandle(hFile);

		WCHAR success_msg[MAX_PATH];
		wsprintf(success_msg, L"Scenario file was created successfully: \n%s", file_name);
		MessageBox(hWnd, success_msg, L"Success", MB_OK | MB_ICONINFORMATION);
	}
}

void on_keydown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 'S')
		game.scenario.world_map.player.pos = game.scenario.world_map.player.pos.sub(from_angle(game.scenario.world_map.player.dir).mul(game.scenario.world_map.player.move_speed));
	else if (wParam == 'W')
		game.scenario.world_map.player.pos = game.scenario.world_map.player.pos.add(from_angle(game.scenario.world_map.player.dir).mul(game.scenario.world_map.player.move_speed));
	else if (wParam == VK_LEFT)
		game.scenario.world_map.player.dir -= game.scenario.world_map.player.rotation_speed;
	else if (wParam == VK_RIGHT)
		game.scenario.world_map.player.dir += game.scenario.world_map.player.rotation_speed;
}

void on_drawitem(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

	if (lpDIS->CtlID == IDC_CREATE_SCENARIO_BUTTON)
	{
		HDC hdc = lpDIS->hDC;
		RECT rect = lpDIS->rcItem;

		BOOL is_pressed = (lpDIS->itemState & ODS_SELECTED);
		BOOL is_hovered = (lpDIS->itemState & ODS_HOTLIGHT);

		COLORREF bg_color = RGB(66, 66, 66);
		COLORREF border_color = RGB(100, 100, 100);
		COLORREF text_color = RGB(255, 255, 255);

		if (is_pressed) {
			bg_color = RGB(40, 40, 40);
			border_color = RGB(150, 150, 150);
		}
		HBRUSH hBrush = CreateSolidBrush(bg_color);
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);

		HPEN hPen = CreatePen(PS_SOLID, 2, border_color);
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hPen);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, text_color);

		WCHAR buttonText[] = L"Create scenario file";
		DrawText(hdc, buttonText, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}
void on_paint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	RECT ClientRect;
	GetClientRect(hWnd, &ClientRect);
	int screen_width = ClientRect.right - ClientRect.left;
	int screen_height = ClientRect.bottom - ClientRect.top;
	if (game.state == Game_State::INPUT_SCENARIO) {
		ShowWindow(hCreateScenarioButton, SW_SHOW);
		RECT instruction_rect = {20, screen_height - 80, screen_width - 20, screen_height - 20};
		game.draw_text_on_screen(hdc, "Drag and drop a scenario file", instruction_rect, RGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_SINGLELINE, 16);
		if (!game.validation_errors.empty()) {
			game.show_validation_errors(hdc, screen_width, screen_height);
		}
	} else if (game.state == Game_State::RUN) {
		ShowWindow(hCreateScenarioButton, SW_HIDE);
		if (game.scenario.world_map.map_state == Map_State::PLAYING) {
			if (screen_width <= 0 || screen_height <= 0) {
				EndPaint(hWnd, &ps);
				return;
			}

			void* render_result = game.raycaster.render_frame(game.scenario.world_map, game.scenario.world_map.player, screen_width, screen_height);
			if (render_result)
			{
				memcpy(game.frame_buffer.memory, render_result, screen_width * screen_height * sizeof(Pixel));
				delete[] static_cast<Pixel*>(render_result);
			}

			game.draw_timer_to_frame_buffer();
			game.DisplayBufferToWindow(hdc, ClientRect);
		} else if (game.scenario.world_map.map_state == Map_State::VICTORY) {
			KillTimer(hWnd, WM_USER + 1);
			ShowWindow(hCreateScenarioButton, SW_HIDE);
			game.Render_Victory_Screen(hdc, screen_width, screen_height);
		} else if (game.scenario.world_map.map_state == Map_State::FAILED) {
			KillTimer(hWnd, WM_USER + 1);
			ShowWindow(hCreateScenarioButton, SW_HIDE);
			game.Render_Failed_Screen(hdc, screen_width, screen_height);
		}
	} 

	EndPaint(hWnd, &ps);
}