#include "Raycaster.h"

Raycaster::Raycaster()
	: grid_size()
{}

void Raycaster::init(Player& player, int map_width, int map_height)
{
	grid_size = Vector2(map_width, map_height);
}



bool Raycaster::inside_map(int cell_x, int cell_y, int cols, int rows)
{
	return cell_x >= 0 && cell_x <= cols && cell_y >= 0 && cell_y <= rows;
}

Ray_Hit Raycaster::cast_ray(WorldMap& world_map, Vector2& p1, Vector2& p2, int cols, int rows)
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

void Raycaster::clear_frame(Pixel *pixels, int screen_width, int screen_height)
{
	memset(pixels, 0, screen_width * screen_height * sizeof(Pixel));
}

void Raycaster::draw_frame(WorldMap& world_map, Pixel* pixels, Player& player, int screen_width, int screen_height)
{
	if (!pixels || screen_width <= 0 || screen_height <= 0) {
		fprintf(stderr, "draw_frame error!\n");
		exit(1);
	}
	Vector2 p1;
	Vector2 p2;
	player.fov_range(p1, p2);

	float render_width = screen_width * 0.5;
	float strip_width = screen_width / render_width;

	for (int x = 0; x < render_width; x++) {
		Vector2 p = p1.lerp(p2, x / render_width);
		Ray_Hit ray_hit = cast_ray(world_map, player.pos, p, grid_size.x, grid_size.y);

		if (ray_hit.hit) {
			COLORREF rgb = world_map.map[ray_hit.hit_pos.y][ray_hit.hit_pos.x].get_rgb_color(); 
			uint8_t r = GetRValue(rgb);
			uint8_t g = GetGValue(rgb);
			uint8_t b = GetBValue(rgb);

			Vector2 ray_dir = p.sub(player.pos).norm();
			Vector2 cam_dir = from_angle(player.dir);
			float perp_dist = ray_hit.ray_len * ray_dir.dot(cam_dir);
			int strip_height = (int)(screen_height / perp_dist);

			int strip_top = (screen_height - strip_height) / 2;
			int strip_bottom = (screen_height + strip_height) / 2;

			for (int y = strip_top; y < strip_bottom; y++) {
				if (y < 0 || y >= screen_height) continue;
				int screen_x = (int)(x * strip_width);
				for (int xi = 0; xi < strip_width; xi++) {
					int index = y * screen_width + (screen_x + xi);
					if (index >= 0 && index < screen_width * screen_height) {
						pixels[index].r = r;
						pixels[index].g = g;
						pixels[index].b = b;
						pixels[index].a = 0;
					}
				}
			}
		}
	}
}


void* Raycaster::render_frame(WorldMap& world_map, Player& player, int screen_width, int screen_height)
{
	Pixel* pixels = new Pixel[screen_width * screen_height]{};
	clear_frame(pixels, screen_width, screen_height);
	draw_frame(world_map, pixels, player, screen_width, screen_height);
	return (void *)pixels;
}