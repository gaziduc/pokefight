#include "anim.h"
#include "window.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>

Anim::Anim(const Window& window, const std::string& filename, const int x, const int y) {
	IMG_Animation* anim = IMG_LoadAnimation(filename.c_str());
	if (anim == nullptr)
		throw std::runtime_error("Error while loading animation:\n" + std::string(IMG_GetError()));

	_pos_dst_x = x;
	_pos_dst_y = y;
	_width = anim->w;
	_height = anim->h;
	_total_frame_count = anim->count;

	int ticks_sum = 0;

	for (int frame_num = 0; frame_num < _total_frame_count; frame_num++) {
		SDL_Texture* texture = SDL_CreateTextureFromSurface(window.get_renderer(), anim->frames[frame_num]);
		if (texture == nullptr) {
			const std::string sdl_error(SDL_GetError());
			IMG_FreeAnimation(anim);
			throw std::runtime_error("Error while loading animation:\n" + sdl_error);
		}
			
		_textures.push_back(texture);
		ticks_sum += anim->delays[frame_num];
		_delays.push_back(ticks_sum);
	}

	IMG_FreeAnimation(anim);
}

Anim::~Anim() {
	for (auto texture : _textures) {
		SDL_DestroyTexture(texture);
	}
}

void Anim::set_pos_dst(const int x, const int y) {
	_pos_dst_x = x;
	_pos_dst_y = y;
}

void Anim::render_anim(const Window& window) {
	int anim_duration = _delays.back();
	int ticks = SDL_GetTicks64() % anim_duration;

	int i = 0;
	while (i < _total_frame_count) {
		if (_delays[i] > ticks)
			break;
		i++;
	}

	SDL_Rect pos_dst = {
		.x = _pos_dst_x,
		.y = _pos_dst_y,
		.w = _width,
		.h = _height
	};

	SDL_RenderCopy(window.get_renderer(), _textures[i], nullptr, &pos_dst);
}


int Anim::get_width() {
	return _width;
}

int Anim::get_height() {
	return _height;
}
