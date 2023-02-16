#pragma once

#include "window.h"
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Anim {
private:
	int _width;
	int _height;
	std::vector<int> _delays;
	std::vector<SDL_Texture*> _textures;
	int _total_frame_count;
	int _pos_dst_x;
	int _pos_dst_y;

public:
	Anim(const Window& window, const std::string& filename, const int x, const int y);
	~Anim();

	void set_pos_dst(const int x, const int y);
	void render_anim(const Window& window);

	int get_width();
	int get_height();
};