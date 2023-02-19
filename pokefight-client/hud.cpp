#include "hud.h"
#include "window.h"
#include "utils.h"
#include <SDL2/SDL.h>

void Hud::show_pokemon_hud(Window& window, const std::string& nickname, const int initial_health, const int current_health, const int pos_dst_x, const int pos_dst_y) {
	SDL_Color text_color = { 0, 0, 0, 255 };
	SDL_Rect pos_dst = { .x = pos_dst_x, .y = pos_dst_y };

	SDL_Texture* nickname_texture = get_text_texture(window, window.get_font(FontSize::SMALL), nickname, text_color, true);
	SDL_QueryTexture(nickname_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
	SDL_RenderCopy(window.get_renderer(), nickname_texture, nullptr, &pos_dst);
	SDL_DestroyTexture(nickname_texture);

	Hud::show_health(window, initial_health, current_health, pos_dst_x, pos_dst_y + 35);
}


void Hud::show_health(Window& window, const int initial_health, const int current_health, const int pos_dst_x, const int pos_dst_y) {
	SDL_Rect pos_dst = { .x = pos_dst_x, .y = pos_dst_y, .w = 160, .h = 20 };

	SDL_SetRenderDrawColor(window.get_renderer(), 0, 0, 0, 255);
	SDL_RenderDrawRect(window.get_renderer(), &pos_dst);

	pos_dst.x++;
	pos_dst.y++;
	pos_dst.w -= 2;
	pos_dst.h -= 2;

	pos_dst.w *= current_health;
	pos_dst.w /= initial_health;

	if (current_health <= initial_health / 6) {
		SDL_SetRenderDrawColor(window.get_renderer(), 237, 28, 36, 255);
	} else if (current_health <= initial_health / 2) {
		SDL_SetRenderDrawColor(window.get_renderer(), 255, 127, 39, 255);
	} else {
		SDL_SetRenderDrawColor(window.get_renderer(), 36, 189, 81, 255);
	}

	SDL_RenderFillRect(window.get_renderer(), &pos_dst);

	pos_dst.x += pos_dst.w;
	pos_dst.w = 158 - pos_dst.w;

	SDL_SetRenderDrawColor(window.get_renderer(), 255, 255, 255, 255);
	SDL_RenderFillRect(window.get_renderer(), &pos_dst);

	pos_dst.x += pos_dst.w;
	pos_dst.y += pos_dst.h / 2;

	SDL_Color text_color = { 0, 0, 0, 255 };
	SDL_Texture* texture = get_text_texture(window, window.get_font(FontSize::VERY_SMALL), std::to_string(current_health) + "/" + std::to_string(initial_health), text_color);
	SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
	pos_dst.x -= pos_dst.w + 5;
	pos_dst.y -= (pos_dst.h / 2 + 2);
	SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
	SDL_DestroyTexture(texture);
}