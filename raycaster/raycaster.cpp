// raycaster.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "raycaster.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_LOADSTRING  100
#define screen_width    800
#define screen_height   600

std::vector<std::vector<int>> world_map = 
{
	{0, 1, 1, 1, 0, 0},
	{0, 0, 0, 1, 0, 0},
	{0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
};


class Vector2 {
private:
public:
	float x, y;
	Vector2()
		: x(0), y(0) {}
	Vector2(float x, float y)
		: x(x), y(y) {}
	Vector2(const Vector2& v)
		: x(v.x), y(v.y) {}
	Vector2 sub(const Vector2 &that)
		{ return Vector2(x - that.x, y - that.y); }
	Vector2 add(const Vector2 &that)
		{ return Vector2(x + that.x, y + that.y); }
	Vector2 mul(const Vector2 &that)
		{ return Vector2(x*that.x, y*that.y); }
	Vector2 mul(float f)
		{ return Vector2(x*f, y*f); }
	float length()
		{ return sqrt(x*x + y*y); }
	Vector2 norm()
		{ float l = length();
	      if (l == 0) return Vector2(0, 0);
		  return Vector2(x/l, y/l); }
	Vector2 scale(float f)
		{ return Vector2(x*f, y*f); }
	Vector2 rot90()
		{ return Vector2(-y, x); }
	Vector2 rot_minus90()
		{ return Vector2(y, -x); }
	Vector2 lerp(const Vector2 &that, float t)
		{ return Vector2(x + t * (that.x - x), y + t * (that.y - y)); }
};

inline Vector2 from_angle(float d)
	{ return Vector2(cos(d), sin(d)); }

inline Vector2 zero()
	{ return Vector2(0, 0); }


class Player {
private:
public:
	Vector2 pos;
	float dir;

	Player(Vector2 p, float d)
		: pos(p), dir(d) {}
	Player()
		: pos(zero()), dir(0.0F) {}
	void fov_range(Vector2& out_p1, Vector2& out_p2, float FOV)
	{
		float l = tan(FOV*0.5);
		Vector2 p = pos.add(from_angle(dir));
		out_p1 = p.sub(pos).rot_minus90().add(p).scale(l);
		out_p2 = p.sub(pos).rot90().add(p).scale(l);
	}

};
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Player player;
Vector2 grid_size;

const float r = 5.0F;
const Vector2 move_speed(0.1F, 0.1F);
const float rotation_speed = 0.05F;
const float FOV = M_PI/2;
const float render_width = 100.0F;
const int FPS = 20;


HPEN red_pen = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
HBRUSH green_brush = CreateSolidBrush(RGB(0, 255, 0));
HBRUSH bg_brush = CreateSolidBrush(RGB(18, 18, 18));
HBRUSH blue_brush = CreateSolidBrush(RGB(0, 0, 255));
HBRUSH wall_brush = CreateSolidBrush(RGB(212, 212, 0));

int on_timer(HWND hwnd)
{
	InvalidateRect(hwnd, NULL, FALSE);
	return 0;
}


Vector2 map_size(std::vector<std::vector<int>> map)
{
	float x = map[0].size();
	float y = map.size();
	return Vector2(x, y);
}

void init(HWND hwnd)
{
	grid_size = map_size(world_map);
	player.pos = grid_size.mul(Vector2(0.78, 0.79));
	player.dir = M_PI * 1.25;
	SetTimer(hwnd, WM_USER + 1, 1000 / FPS, 0);
}

void stroke_line(HDC hdc, const Vector2 &p1, const Vector2 &p2, float scale)
{
	MoveToEx(hdc, p1.x * scale, p1.y * scale, nullptr);
	LineTo(hdc, p2.x * scale, p2.y * scale);
}

float cast_ray(Vector2& p1, Vector2& p2, int cols, int rows) 
{
    float dx = p2.x - p1.x; // ray dirx
    float dy = p2.y - p1.y; // ray diry
    
    int stepx = 0;
    int stepy = 0;


    if (dx == 0) dx = 1e-6f;
    if (dy == 0) dy = 1e-6f;

    float sx = sqrt(1 + (dy*dy)/(dx*dx));
    float sy = sqrt(1 + (dx*dx)/(dy*dy));

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

    while (!tile_found && distance < max_distance) {
        float pcx, pcy;
        if (ray_len_x < ray_len_y) {
            distance = ray_len_x;
            map_check_x += stepx;
            ray_len_x += sx;

        } else {
            distance = ray_len_y;
            map_check_y += stepy;
            ray_len_y += sy;
        }

        if (map_check_x >= 0 && map_check_x < cols && map_check_y >= 0 && map_check_y < rows) {
            if (map_check_y < rows && map_check_x < cols && 
                world_map[map_check_y][map_check_x] == 1) {
                tile_found = true;
            }
        }
    }

    return distance;
}


void draw_grid(HDC hdc, int cols, int rows, float scale)
{
	SelectObject(hdc, red_pen);

	for (int x = 0; x <= cols; x++) {
		stroke_line(hdc, Vector2(x, 0), Vector2(x, cols), scale);
	}

	for (int y = 0; y <= rows; y++) {
		stroke_line(hdc, Vector2(0, y), Vector2(rows, y), scale);
	}
}


void fill_walls(HDC hdc, int cols, int rows, float scale, std::vector<std::vector<int>> map)
{
	SelectObject(hdc, blue_brush);

	int wall_pos_x = 0;
	int wall_pos_y = 0;
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			if (map[y][x] == 1) {
				wall_pos_x = x * scale;
				wall_pos_y = y * scale;
				Rectangle(hdc, wall_pos_x, wall_pos_y, wall_pos_x + scale, wall_pos_y + scale);
			}
		}
	}
}



void redraw_frame(HDC hdc)
{
	SelectObject(hdc, bg_brush);
	Rectangle(hdc, 0, 0, screen_width, screen_height);
}


void draw_minimap(HDC hdc, float scale, std::vector<std::vector<int>> map)
{	
	SelectObject(hdc, bg_brush);
	Rectangle(hdc, 0, 0, grid_size.x * scale, grid_size.y * scale);

	draw_grid(hdc, grid_size.x, grid_size.y, scale);
	fill_walls(hdc, grid_size.x, grid_size.y, scale, map);
    
	SelectObject(hdc, green_brush);
	Ellipse(hdc, player.pos.x * scale - r, player.pos.y * scale - r, player.pos.x * scale + r, player.pos.y * scale + r);
	
	//drawing camera
	Vector2 p1;
	Vector2 p2;
	player.fov_range(p1, p2, FOV);
	stroke_line(hdc, p1, p2, scale);
	stroke_line(hdc, player.pos, p1, scale);
	stroke_line(hdc, player.pos, p2, scale);
	
	return;
}

void render(HDC hdc, float scale, std::vector<std::vector<int>> map)
{
	float strip_width = screen_width/render_width;
	Vector2 p1;
	Vector2 p2;
	player.fov_range(p1, p2, FOV);
	SelectObject(hdc, wall_brush);
	for (int x = 0; x < render_width; x++) {
		Vector2 p = p1.lerp(p2, x/render_width);
		float d = cast_ray(player.pos, p, grid_size.x, grid_size.y);
		if (d < 100.0F) {
			float strip_height = screen_height / d;
			float strip_y = (screen_height - strip_height) * 0.5;
			Rectangle(hdc, x*strip_width, strip_y, (x + 1) * strip_width, strip_y + strip_height);
		}
	}

	return;
}

void draw_frame(HDC hdc)
{
	redraw_frame(hdc);
	float minimap_scale_factor = 0.3F;
	float scale = minimap_scale_factor * min(screen_width / grid_size.x, screen_height / grid_size.y);
	render(hdc, scale, world_map);
	draw_minimap(hdc, scale, world_map);
}


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
	window_rect.bottom = screen_height;
	window_rect.right  = screen_width;

	AdjustWindowRect(&window_rect,  WS_OVERLAPPEDWINDOW, TRUE);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == 0)
	{
		return FALSE;
	}
	
	init(hWnd);
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
		case WM_KEYDOWN:
			{
				if (wParam == 'S')
					player.pos = player.pos.sub(from_angle(player.dir).mul(move_speed));
				else if (wParam == 'W')
					player.pos = player.pos.add(from_angle(player.dir).mul(move_speed));
				else if (wParam == VK_LEFT)
					player.dir -= rotation_speed;
				else if (wParam == VK_RIGHT)
					player.dir += rotation_speed;
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
