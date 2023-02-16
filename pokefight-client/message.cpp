
#include "message.h"
#include "window.h"
#include "utils.h"
#include <string>
#include <SDL2/SDL.h>

void Message::show_message(Window& window, const std::string& message, const bool is_utf8) {
	int finished = false;
	Uint64 first_ticks = SDL_GetTicks64();
	SDL_Color text_color = { 0, 0, 0, 255 };

	while (!finished) {
		window.update_events();

		if (window.is_quit())
			exit(0);

		if (window.is_enter_down()) {
			window.press_up_key(SDL_SCANCODE_RETURN);
			window.press_up_key(SDL_SCANCODE_KP_ENTER);

			finished = true;
		}

		Uint64 ticks = SDL_GetTicks64();
		int num_letters_to_show = (ticks - first_ticks) / 20;
		if (num_letters_to_show >= message.size()) {
			finished = true;
		}

		window.render_clear();

		SDL_Rect pos_dst = { .x = 20, .y = window.get_height() - 70, .w = window.get_height(), .h = 70 };
		SDL_RenderDrawRect(window.get_renderer(), &pos_dst);

		std::string to_show(message.substr(0, num_letters_to_show));
		if (!to_show.empty()) {
			SDL_Texture* texture = get_text_texture(window, window.get_font(FontSize::NORMAL), to_show, text_color);
			SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
			SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
			SDL_DestroyTexture(texture);
		}

		window.render_present();
	}


	first_ticks = SDL_GetTicks64();
	Uint64 ticks = first_ticks;

	while (ticks - first_ticks < 1000) {
		window.update_events();

		if (window.is_quit())
			exit(0);

		window.render_clear();

		SDL_Rect pos_dst = { .x = 20, .y = window.get_height() - 70, .w = window.get_height(), .h = 70 };
		SDL_RenderDrawRect(window.get_renderer(), &pos_dst);

		SDL_Texture* texture = get_text_texture(window, window.get_font(FontSize::NORMAL), message, text_color);
		SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
		SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
		SDL_DestroyTexture(texture);

		window.render_present();

		ticks = SDL_GetTicks64();
	}

}

