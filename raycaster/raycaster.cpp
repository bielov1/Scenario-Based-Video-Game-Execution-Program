// raycaster.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "raycaster.h"
#include <string>
#include <sstream>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_LOADSTRING  100
#define screen_width    800
#define screen_height   800
#define grid_rows       10
#define grid_cols		10

int world_map[grid_cols][grid_rows] = 
{
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0}
};

class Vector2 {
private:
public:
	Vector2(float x, float y)
		: x(x), y(y)
	{ }
	Vector2 sub(Vector2 &that)
		{ return Vector2(x - that.x, y - that.y); }

	Vector2 add(Vector2 &that)
		{ return Vector2(x + that.x, y + that.y); }
	float length()
		{ return sqrt(x*x + y*y); }
	Vector2 norm()
		{ float l = length();
	      if (l == 0) return Vector2(0, 0);
		  return Vector2(x/l, y/l); }
	float x, y;
};


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Vector2 *p1 = new Vector2(grid_cols*0.43, grid_rows*0.32);			    // x and  y start position

HPEN red_pen = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
HBRUSH green_brush = CreateSolidBrush(RGB(0, 255, 0));
HBRUSH bg_brush = CreateSolidBrush(RGB(18, 18, 18));
HBRUSH blue_brush = CreateSolidBrush(RGB(0, 0, 255));

const int wall_width = screen_width/grid_cols;
const int wall_height = screen_height/grid_rows;
const float global_scale = min(screen_width / grid_cols, screen_height / grid_rows);
const float r = 5.0F;
const float move_speed = 0.2F;

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
	RECT  window_rect;
	window_rect.left   = 0;
	window_rect.top    = 0;
	window_rect.bottom =	screen_height;
	window_rect.right  = screen_width;

	AdjustWindowRect(&window_rect,  WS_OVERLAPPEDWINDOW, TRUE);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == 0)
	{
		return FALSE;
	}
	SetTimer(hWnd, WM_USER + 1, 50, 0);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

int on_timer(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, FALSE);
	return 0;
}
void stroke_line(HDC hdc, const Vector2 &p1, const Vector2 &p2)
{
	MoveToEx(hdc, p1.x * global_scale, p1.y * global_scale, nullptr);
	LineTo(hdc, p2.x * global_scale, p2.y * global_scale);
}

void draw_grid(HDC hdc)
{
	SelectObject(hdc, red_pen);

	for (int x = 0; x <= grid_cols; x++) {
		stroke_line(hdc, Vector2(x, 0), Vector2(x, grid_cols));
	}

	for (int y = 0; y <= grid_rows; y++) {
		stroke_line(hdc, Vector2(0, y), Vector2(grid_rows, y));
	}
}

void fill_walls(HDC hdc)
{
	SelectObject(hdc, blue_brush);

	float wall_size = wall_height;
	for (int i = 0; i < grid_cols; i++) {
		for (int j = 0; j < grid_rows; j++) {
			if (world_map[i][j] == 1) {
				int wall_pos_x = i * global_scale;
				int wall_pos_y = j * global_scale;
				Rectangle(hdc, wall_pos_x, wall_pos_y, wall_pos_x + wall_size, wall_pos_y + wall_size);
			}
		}
	}
}

void line_analyzer(HDC hdc, Vector2& p1, Vector2& p2)
{
	float dx = p2.x - p1.x; // ray dirx
	float dy = p2.y - p1.y; // ray diry
	
	int stepx = 0;
	int stepy = 0;

	float sx = sqrt(1 + (dy*dy/dx*dx));
	float sy = sqrt(1 + (dx*dx/dy*dy));

	float ray_len_x = 0;
	float ray_len_y = 0;
	
	int map_check_x = int(p1.x);
	int map_check_y = int(p1.y);

	if (dx < 0) {
		stepx = -1;
		ray_len_x = (p1.x - float(map_check_x)) * sx;
	} else {
		stepx = 1;
		ray_len_x = (float(map_check_x + 1) - p1.x) * sx;
	}

	if (dy < 0) {
		stepy = -1;
		ray_len_y = (p1.y - float(map_check_y)) * sy;
	} else {
		stepy = 1;
		ray_len_y = (float(map_check_y + 1) - p1.y) * sy;
	}
	bool tile_found = false;
	float max_distance = 100.0f;
	float distance = 0.0f;
	
	SelectObject(hdc, green_brush);

	while (!tile_found && distance < max_distance) {
		float pcx, pcy;
		if (ray_len_x < ray_len_y) {
			distance = ray_len_x;

			pcx = (stepx > 0) ? float(map_check_x + 1) : float(map_check_x);
			float t = (pcx - p1.x) / dx;
			pcy = p1.y + t * dy;

			map_check_x += stepx;
			ray_len_x += sx;

		} else {
			distance = ray_len_y;

			pcy = (stepy > 0) ? float(map_check_y + 1) : float(map_check_y);
			float t = (pcy - p1.y) / dy;
			pcx = p1.x + t * dx;

			map_check_y += stepy;
			ray_len_y += sy;
		}


		if (map_check_x >= 0 && map_check_x < grid_cols && map_check_y >= 0 && map_check_y < grid_rows) {
			Ellipse(hdc, pcx * global_scale - r, pcy * global_scale - r, 
					pcx * global_scale + r, pcy * global_scale + r);
			if (world_map[map_check_x][map_check_y] == 1) {
				tile_found = true;
			}
		}
	}

	return;
}

void redraw_frame(HDC hdc)
{
	SelectObject(hdc, bg_brush);
	Rectangle(hdc, 0, 0, screen_width, screen_height);
}

void draw_frame(HDC hdc)
{
	redraw_frame(hdc);
	POINT mouse_point;
	if (GetCursorPos(&mouse_point) == 0) {
		OutputDebugStringA("unable to read cursor point");
	}
	
	float cursor_col_point = mouse_point.x / global_scale;
	float cursor_row_point = mouse_point.y / global_scale;
	draw_grid(hdc);
	fill_walls(hdc);
    
	SelectObject(hdc, green_brush);
	
	Ellipse(hdc, p1->x * global_scale - r, p1->y * global_scale - r, p1->x * global_scale + r, p1->y * global_scale + r);

	Vector2 *p2 = new Vector2(cursor_col_point, cursor_row_point);
	Ellipse(hdc, p2->x *global_scale - r, p2->y * global_scale - r, p2->x * global_scale + r, p2->y * global_scale + r);
	stroke_line(hdc, *p1, *p2);

	line_analyzer(hdc, *p1, *p2);

	delete p2;
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
		case WM_KEYDOWN:
			{
				if (wParam == 'S')
					p1->y += move_speed;
				else if (wParam == 'W')
					p1->y -= move_speed;
				else if (wParam == 'A')
					p1->x -= move_speed;
				else if (wParam == 'D')
					p1->x += move_speed;
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
				draw_frame(hdc);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_TIMER:
			if (wParam == WM_USER + 1)
				return on_timer(hWnd);
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
