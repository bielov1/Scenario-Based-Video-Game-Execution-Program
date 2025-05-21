#include "Config.h"

// AColor
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
	: R(r), G(g), B(b)
{
}
//------------------------------------------------------------------------------------------------------------

// AsConfig
const AColor AsConfig::bg_color(18, 18, 18);
const AColor AsConfig::green_color(0, 255, 0);
const AColor AsConfig::blue_color(0, 0, 255);
const AColor AsConfig::red_color(255, 0, 0);
const AColor AsConfig::default_color(255, 255, 255);

HPEN AsConfig::bg_pen;
HPEN AsConfig::green_pen;
HPEN AsConfig::blue_pen;
HPEN AsConfig::red_pen;
HPEN AsConfig::default_pen;

HBRUSH AsConfig::bg_brush; 
HBRUSH AsConfig::green_brush;
HBRUSH AsConfig::blue_brush;
HBRUSH AsConfig::red_brush;
HBRUSH AsConfig::default_brush;

//------------------------------------------------------------------------------------------------------------
void AsConfig::setup_colors()
{
	AsConfig::create_pen_brush(AsConfig::bg_color, bg_pen, bg_brush);
	AsConfig::create_pen_brush(AsConfig::green_color, green_pen, green_brush);
	AsConfig::create_pen_brush(AsConfig::blue_color, blue_pen, blue_brush);
	AsConfig::create_pen_brush(AsConfig::red_color, red_pen, red_brush);
	AsConfig::create_pen_brush(AsConfig::default_color, default_pen, default_brush);
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::create_pen_brush(const AColor &color, HPEN &pen, HBRUSH &brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
	brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------