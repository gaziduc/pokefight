
#include "main.h"
#include "message.h"
#include "window.h"
#include "utils.h"
#include <string>
#include <SDL2/SDL.h>

void Message::show_message_battle(Window& window, const std::string& message, const bool is_utf8, const Pokemons& pokemons, const std::shared_ptr<Player>& my_player_ptr, const std::vector<std::shared_ptr<Player>>& other_players_ptr) {
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
		render_battlefield(window, pokemons, my_player_ptr, other_players_ptr);

		SDL_Rect pos_dst = { .x = 80, .y = window.get_height() - 80, .w = 0, .h = 0 };

		std::string to_show(utf8_substr(message, 0, num_letters_to_show));
		if (!to_show.empty()) {
			SDL_Texture* texture = get_text_texture(window, window.get_font(FontSize::NORMAL), to_show, text_color, is_utf8);
			SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
			pos_dst.y -= pos_dst.h;
			SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
			SDL_DestroyTexture(texture);
		}

		window.render_present();
	}


	first_ticks = SDL_GetTicks64();
	Uint64 ticks = first_ticks;

	while (ticks - first_ticks < 1255) {
		window.update_events();

		if (window.is_quit())
			exit(0);

		window.render_clear();
		render_battlefield(window, pokemons, my_player_ptr, other_players_ptr);

		SDL_Rect pos_dst = { .x = 80, .y = window.get_height() - 80, .w = 0, .h = 0 };
		SDL_RenderDrawRect(window.get_renderer(), &pos_dst);

		if (ticks - first_ticks >= 1000) {
			text_color.a = first_ticks - ticks + 1255;
		}

		SDL_Texture* texture = get_text_texture(window, window.get_font(FontSize::NORMAL), message, text_color, is_utf8);
		SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
		pos_dst.y -= pos_dst.h;
		SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
		SDL_DestroyTexture(texture);

		window.render_present();

		ticks = SDL_GetTicks64();
	}

}

