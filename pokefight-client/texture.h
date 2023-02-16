#pragma once

#include <SDL2/SDL.h>
#include <string>

// forward declaration
class Window;

class Texture {
private:
	SDL_Texture* _texture;
	int _pos_dst_x;
	int _pos_dst_y;
	int _width;
	int _height;

public:
	Texture(const Window& window, const std::string& filename);
	~Texture();

	void set_pos_dst(const int x, const int y);
	void render(const Window& window);
	int get_width();
	int get_height();
};