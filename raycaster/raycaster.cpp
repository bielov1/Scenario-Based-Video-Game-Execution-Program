#include "Raycaster.h"

Raycaster::Raycaster()
	: grid_size()
{}

void Raycaster::init(HWND hwnd)
{
	world_map.init(10, 10);
	grid_size = Vector2(world_map.map_width, world_map.map_height);
	player.init(100, grid_size.mul(Vector2(0.55, 0.95)), M_PI * 1.25, "STAND");
	SetTimer(hwnd, WM_USER + 1, 1000 / AsConfig::FPS, 0);
}

int Raycaster::on_timer(HWND hwnd)
{
	InvalidateRect(hwnd, NULL, FALSE);
	return 0;
}

void Raycaster::stroke_line(HDC hdc, const Vector2 &p1, const Vector2 &p2, float scale)
{
	MoveToEx(hdc, p1.x * scale, p1.y * scale, nullptr);
	LineTo(hdc, p2.x * scale, p2.y * scale);
}

bool Raycaster::inside_map(int cell_x, int cell_y, int cols, int rows)
{
	return cell_x >= 0 && cell_x <= cols && cell_y >= 0 && cell_y <= rows;
}

Ray_Hit Raycaster::cast_ray(Vector2& p1, Vector2& p2, int cols, int rows)
{
	float dx = p2.x - p1.x; // ray dirx
	float dy = p2.y - p1.y; // ray diry

	int stepx = 0;
	int stepy = 0;

	if (dx == 0) dx = 1e-6f;
	if (dy == 0) dy = 1e-6f;

	float sx = sqrt(1 + (dy*dy)/(dx*dx));
	float sy = sqrt(1 + (dx*dx)/(dy*dy));
	float ray_len;

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

	int side;
	while (true) {
		if (ray_len_x < ray_len_y) {
			map_check_x += stepx;
			ray_len_x += sx;
			side = 0;

		} else {
			map_check_y += stepy;
			ray_len_y += sy;
			side = 1;
		}

		if (map_check_x >= 0 && map_check_x < cols && 
			map_check_y >= 0 && map_check_y < rows &&
			world_map.map[map_check_y][map_check_x].render) {
			ray_len = (side == 0) ? (ray_len_x - sx) : (ray_len_y - sy);
			return Ray_Hit(Vector2(map_check_x, map_check_y), ray_len, true);
		}

		if (!inside_map(map_check_x, map_check_y, cols, rows)) break;
	}

	return Ray_Hit(Vector2(0, 0), 0.0F, false);
}

void Raycaster::draw_grid(HDC hdc, int cols, int rows, float scale)
{
	SelectObject(hdc, AsConfig::red_pen);

	for (int x = 0; x <= cols; x++) {
		stroke_line(hdc, Vector2(x, 0), Vector2(x, cols), scale);
	}

	for (int y = 0; y <= rows; y++) {
		stroke_line(hdc, Vector2(0, y), Vector2(rows, y), scale);
	}
}

void Raycaster::fill_walls(HDC hdc, int cols, int rows, float scale)
{
	int wall_pos_x = 0;
	int wall_pos_y = 0;
	HBRUSH color;
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			if (world_map.map[y][x].render) {
				wall_pos_x = static_cast<int>(x * scale);
				wall_pos_y = static_cast<int>(y * scale);
				color = world_map.map[y][x].wall_color;
				SelectObject(hdc, color);
				Rectangle(hdc, wall_pos_x, wall_pos_y, wall_pos_x + scale, wall_pos_y + scale);
			}
		}
	}
}

void Raycaster::redraw_frame(HDC hdc)
{
	SelectObject(hdc, AsConfig::bg_brush);
	Rectangle(hdc, 0, 0, screen_width, screen_height);
}

void Raycaster::draw_minimap(HDC hdc, Player& player, float scale)
{	
	SelectObject(hdc, AsConfig::bg_brush);
	Rectangle(hdc, 0, 0, grid_size.x * scale, grid_size.y * scale);

	draw_grid(hdc, grid_size.x, grid_size.y, scale);
	fill_walls(hdc, grid_size.x, grid_size.y, scale);

	SelectObject(hdc, AsConfig::green_brush);
	Ellipse(hdc, player.pos.x * scale - r, player.pos.y * scale - r, player.pos.x * scale + r, player.pos.y * scale + r);

	//drawing camera
	Vector2 p1;
	Vector2 p2;
	player.fov_range(p1, p2);
	stroke_line(hdc, p1, p2, scale);
	stroke_line(hdc, player.pos, p1, scale);
	stroke_line(hdc, player.pos, p2, scale);
}

void Raycaster::render(HDC hdc, Player& player, float scale)
{
	Vector2 p1;
	Vector2 p2;
	player.fov_range(p1, p2);

	float render_width = 100.0F;
	float strip_width = screen_width/render_width;
	RECT strip_rect;
	for (int x = 0; x < render_width; x++) {
		Vector2 p = p1.lerp(p2, x/render_width);
		Ray_Hit ray_hit = cast_ray(player.pos, p, grid_size.x, grid_size.y);
		if (ray_hit.hit) {
			HBRUSH color = world_map.map[ray_hit.hit_pos.y][ray_hit.hit_pos.x].wall_color;
			Vector2 ray_dir = p.sub(player.pos).norm();
			Vector2 cam_dir = from_angle(player.dir);
			float perp_dist = ray_hit.ray_len * ray_dir.dot(cam_dir);
			int strip_height = (int)(screen_height / perp_dist);

			strip_rect.left = static_cast<long>(x*strip_width);
			strip_rect.top = static_cast<long>((screen_height - strip_height) * 0.5);
			strip_rect.right = static_cast<long>((x + 1) * strip_width);
			strip_rect.bottom = static_cast<long>((screen_height + strip_height) * 0.5);

			SelectObject(hdc, color);
			Rectangle(hdc, strip_rect.left, strip_rect.top, strip_rect.right, strip_rect.bottom);
			FrameRect(hdc, &strip_rect, color);
		}
	}
}

void Raycaster::render_game(HDC hdc)
{
	redraw_frame(hdc);
	float minimap_scale_factor = 0.3F;
	float scale = minimap_scale_factor * min(screen_width / grid_size.x, screen_height / grid_size.y);
	render(hdc, player, scale);
	draw_minimap(hdc, player, scale);
}

Player& Raycaster::playerInstance()
{ return player;}

WorldMap& Raycaster::worldmapInstance()
{ return world_map; }