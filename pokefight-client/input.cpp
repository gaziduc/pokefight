#include "input.h"
#include "events.h"
#include "window.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <string>

Input::Input(const std::string& title, TTF_Font* font, const std::string& default_input_value) {
	_title = title;
	_font = font;
	_current_input = default_input_value;
}

int Input::show_input_menu(Window& window) {
	int res = InputEndAction::ENTER;

	while (true) {
		window.update_events();

		if (window.is_quit())
			exit(0);

		if (window.is_key_down(SDL_SCANCODE_BACKSPACE)) {
			pop_back_utf8(_current_input);

			window.press_up_key(SDL_SCANCODE_BACKSPACE);
		} else {
			_current_input += window.get_text();
		}

		if (SDL_GetModState() & KMOD_CTRL && window.is_key_down(SDL_SCANCODE_V)) {
			_current_input += SDL_GetClipboardText();

			window.press_up_key(SDL_SCANCODE_V);
		}

		if (window.is_enter_down()) {
			window.press_up_key(SDL_SCANCODE_RETURN);
			window.press_up_key(SDL_SCANCODE_KP_ENTER);
			break;
		}

		if (window.is_escape_down()) {
			window.press_up_key(SDL_SCANCODE_ESCAPE);
			res = InputEndAction::ESCAPE;
			break;
		}

		window.render_clear();
		render_input(window);
		window.render_present();
	}

	return res;
}

std::string Input::get_current_input() const {
	return _current_input;
}


void Input::render_input(const Window& window) {
	SDL_Rect pos_dst = {
		.x = window.get_width() / 2 - 360,
		.y = window.get_height() / 2 - 60,
		.w = 720,
		.h = 120
	};

	SDL_SetRenderDrawColor(window.get_renderer(), 255, 255, 255, 255);
	SDL_RenderFillRect(window.get_renderer(), &pos_dst);
	SDL_SetRenderDrawColor(window.get_renderer(), 0, 0, 0, 255);
	SDL_RenderDrawRect(window.get_renderer(), &pos_dst);
	pos_dst.x--;
	pos_dst.y--;
	SDL_RenderDrawRect(window.get_renderer(), &pos_dst);
	pos_dst.x += 2;
	pos_dst.y += 2;
	SDL_RenderDrawRect(window.get_renderer(), &pos_dst);
	pos_dst.x--;
	pos_dst.y--;

	// 4 pokeballs
	Texture* pokeball = window.get_texture(Picture::POKEBALL_ICON);
	pokeball->set_pos_dst(pos_dst.x - pokeball->get_width() / 2, pos_dst.y - pokeball->get_height() / 2);
	pokeball->render(window);

	pokeball->set_pos_dst(pos_dst.x + pos_dst.w - pokeball->get_width() / 2, pos_dst.y - pokeball->get_height() / 2);
	pokeball->render(window);

	pokeball->set_pos_dst(pos_dst.x - pokeball->get_width() / 2, pos_dst.y + pos_dst.h - pokeball->get_height() / 2);
	pokeball->render(window);

	pokeball->set_pos_dst(pos_dst.x + pos_dst.w - pokeball->get_width() / 2, pos_dst.y + pos_dst.h - pokeball->get_height() / 2);
	pokeball->render(window);

	pos_dst.y += 10;
	pos_dst.x += 75;

	SDL_Texture* title_texture = get_text_texture(window, _font, _title, _text_color);
	SDL_QueryTexture(title_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
	SDL_RenderCopy(window.get_renderer(), title_texture, nullptr, &pos_dst);
	SDL_DestroyTexture(title_texture);

	pos_dst.y += 50;

	SDL_Texture* input_texture = get_text_texture(window, _font, _current_input + (SDL_GetTicks64() % 1000 < 500 ? '_' : ' '), _text_color, true);
	SDL_QueryTexture(input_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
	SDL_RenderCopy(window.get_renderer(), input_texture, nullptr, &pos_dst);
	SDL_DestroyTexture(input_texture);
}