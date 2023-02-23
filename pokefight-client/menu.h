#pragma once

#include "events.h"
#include "window.h"
#include <map>
#include <string>
#include <optional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum MainMenu {
	FIGHT_ONLINE_PLAYERS = 0,
	FIGHT_AGAINST_COMPUTER = 1,
	SETTINGS = 2,
	QUIT = 3
};

class Menu {
private:
	std::optional<std::string> _title;
	TTF_Font* _font;
	
protected:
	int _selected_choice_num;
	std::map<int, std::string> _choices;
	int _width;

public:
	Menu(const std::optional<std::string>& title, TTF_Font* font, const int width);

	void add_choice(const int choice_num, const std::string& str);
	void update_selected_choice_from_events(Events& events);
	void render_menu(const Window& window);
	int get_selected_choice_num() const;
	std::string get_selected_choice_str() const;
	void clear_choices();

	static void render_rect_and_pokeballs(const Window& window, SDL_Rect* pos_dst);

	const SDL_Color _text_color = { 0, 0, 0, 255 };
};


class AttackMenu: public Menu {
private:
	std::vector<Type> _attack_types;

public:
	AttackMenu(const std::optional<std::string>& title, TTF_Font* font, const int width);

	void add_choice(const int choice_num, const std::string& str, const Type attack_type);
	void render_menu(const Window& window);
};