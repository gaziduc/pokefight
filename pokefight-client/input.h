#pragma once

#include "events.h"
#include "window.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


enum InputEndAction {
	ENTER = 0,
	ESCAPE = 1
};

class Input {
private:
	std::string _title;
	std::string _current_input;
	TTF_Font* _font;
	bool _replace_forbidden_chars;

	void render_input(const Window& window);

public:
	Input(const std::string& title, TTF_Font* font, const std::string& default_input_value, const bool replace_spaces);
	
	int show_input_menu(Window& window, std::shared_ptr<Texture> title_texture, std::shared_ptr<Texture> menu_texture);
	std::string get_current_input() const;
	
	const SDL_Color _text_color = { 0, 0, 0, 255 };
};