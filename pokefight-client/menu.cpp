#include "menu.h"
#include "window.h"
#include "events.h"
#include "utils.h"
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <memory>
#include <optional>

Menu::Menu(const std::optional<std::string>& title, TTF_Font *font, const int width) {
	_title = title;
	_font = font;
	_selected_choice_num = 0;
	_width = width;
}

void Menu::add_choice(const int choice_num, const std::string& str) {
	_choices.emplace(choice_num, str);
}

void Menu::update_selected_choice_from_events(Events& events) {
	if (events.is_key_down(SDL_SCANCODE_UP)) {
		if (_selected_choice_num > 0)
			_selected_choice_num--;

		events.press_up_key(SDL_SCANCODE_UP);
	}

	if (events.is_key_down(SDL_SCANCODE_DOWN)) {
		if (_selected_choice_num < _choices.size() - 1)
			_selected_choice_num++;

		events.press_up_key(SDL_SCANCODE_DOWN);
	}
}

void Menu::render_menu(const Window& window) {
	const int num_choices = (int) _choices.size();

	const int y = window.get_height() - (int)(num_choices + 2) * 50;
	const int h = window.get_height() - y - 50;


	SDL_Rect pos_dst = {
		.x = window.get_width() / 2 - _width / 2,
		.y = _title.has_value() ? y - 60 : y,
		.w = _width,
		.h = _title.has_value() ? h + 60 : h
	};

	Menu::render_rect_and_pokeballs(window, &pos_dst);

	pos_dst.y += 28;
	

	// Title
	if (_title.has_value()) {
		pos_dst.x += 40;
		SDL_Texture* texture = get_text_texture(window, _font, _title.value(), _text_color);
		SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
		SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
		SDL_DestroyTexture(texture);

		pos_dst.y += 60;
		pos_dst.x += 35;
	}
	else {
		pos_dst.x += 75;
	}
	
	// Choices
	for (size_t choice_num = 0; choice_num < num_choices; choice_num++) {
		std::string choice = _choices[choice_num];

		SDL_Texture* texture = get_text_texture(window, _font, choice, _text_color, true);
		SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
		SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
		SDL_DestroyTexture(texture);

		if (_selected_choice_num == choice_num) {
			Texture* selected = window.get_texture(Picture::MENU_SELECTED);
			selected->set_pos_dst(pos_dst.x - 35, pos_dst.y + 15);
			selected->render(window);
		}	

		pos_dst.y += 50;
	}
}


int Menu::get_selected_choice_num() const {
	return _selected_choice_num;
}

std::string Menu::get_selected_choice_str() const {
	return _choices.at(_selected_choice_num);
}

void Menu::clear_choices() {
	_choices.clear();
	_selected_choice_num = 0;
}


void Menu::render_rect_and_pokeballs(const Window& window, SDL_Rect *pos_dst) {
	SDL_SetRenderDrawColor(window.get_renderer(), 255, 255, 255, 255);
	SDL_RenderFillRect(window.get_renderer(), pos_dst);
	SDL_SetRenderDrawColor(window.get_renderer(), 0, 0, 0, 255);
	SDL_RenderDrawRect(window.get_renderer(), pos_dst);
	pos_dst->x--;
	pos_dst->y--;
	SDL_RenderDrawRect(window.get_renderer(), pos_dst);
	pos_dst->x += 2;
	pos_dst->y += 2;
	SDL_RenderDrawRect(window.get_renderer(), pos_dst);
	pos_dst->x--;
	pos_dst->y--;

	// 4 pokeballs
	Texture* pokeball = window.get_texture(Picture::POKEBALL_ICON);
	pokeball->set_pos_dst(pos_dst->x - pokeball->get_width() / 2, pos_dst->y - pokeball->get_height() / 2);
	pokeball->render(window);

	pokeball->set_pos_dst(pos_dst->x + pos_dst->w - pokeball->get_width() / 2, pos_dst->y - pokeball->get_height() / 2);
	pokeball->render(window);

	pokeball->set_pos_dst(pos_dst->x - pokeball->get_width() / 2, pos_dst->y + pos_dst->h - pokeball->get_height() / 2);
	pokeball->render(window);

	pokeball->set_pos_dst(pos_dst->x + pos_dst->w - pokeball->get_width() / 2, pos_dst->y + pos_dst->h - pokeball->get_height() / 2);
	pokeball->render(window);
}


AttackMenu::AttackMenu(const std::optional<std::string>& title, TTF_Font* font, const int width) : Menu(title, font, width) {

}

void AttackMenu::add_choice(const int choice_num, const std::string& str, const Type attack_type) {
	_choices.emplace(choice_num, str);
	_attack_types.emplace_back(attack_type);
}

void AttackMenu::render_menu(const Window& window) {
	Menu::render_menu(window);

	const int num_choices = (int)_choices.size();

	const int y = window.get_height() - (int)(num_choices + 2) * 50;
	const int h = window.get_height() - y - 50;

	// Choices
	for (size_t choice_num = 0; choice_num < num_choices; choice_num++) {
		Picture type_pic = (Picture) _attack_types[choice_num];
		window.get_texture(type_pic)->set_pos_dst(window.get_width() / 2 + _width / 2 - window.get_texture(type_pic)->get_width() - 50, y + 35 + choice_num * 50);
		window.get_texture(type_pic)->render(window);
	}
}