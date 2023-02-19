
#include "texture.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdexcept>
#include <iostream>

Texture::Texture(const Window& window, const std::string& filename) {
	SDL_Surface *surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
		throw std::runtime_error("Error while loading image:\n" + std::string(IMG_GetError()));

	_texture = SDL_CreateTextureFromSurface(window.get_renderer(), surface);
	if (_texture == nullptr)
		throw std::runtime_error("Error while loading image:\n" + std::string(SDL_GetError()));

	SDL_FreeSurface(surface);

	SDL_QueryTexture(_texture, nullptr, nullptr, &_width, &_height);
}

Texture::~Texture() {
	SDL_DestroyTexture(_texture);
}

void Texture::set_pos_dst(const int x, const int y) {
	_pos_dst_x = x;
	_pos_dst_y = y;
}

void Texture::render(const Window& window) {
	SDL_Rect pos_dst = {
		.x = _pos_dst_x,
		.y = _pos_dst_y,
		.w = _width,
		.h = _height
	};

	SDL_RenderCopy(window.get_renderer(), _texture, nullptr, &pos_dst);
}

void Texture::render_without_pos_dst(const Window& window) {
	SDL_RenderCopy(window.get_renderer(), _texture, nullptr, nullptr);
}

int Texture::get_width() {
	return _width;
}

int Texture::get_height() {
	return _height;
}