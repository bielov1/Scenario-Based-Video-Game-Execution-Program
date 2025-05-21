#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	AColor(unsigned char r, unsigned char g, unsigned char b);

	unsigned char R, G, B;
};
//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void setup_colors();
	static void create_pen_brush(const AColor &color, HPEN &pen, HBRUSH &brush);
	static void create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);

	static const AColor bg_color, green_color, blue_color, red_color, default_color;
	static HPEN bg_pen, green_pen, blue_pen, red_pen, default_pen;
	static HBRUSH bg_brush, green_brush, blue_brush, red_brush, default_brush;
	
	static const int FPS = 60;
};
//------------------------------------------------------------------------------------------------------------

