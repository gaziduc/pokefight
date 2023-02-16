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
	std::map<int, std::string> _choices;
	int _selected_choice;
	TTF_Font* _font;
	

public:
	Menu(const std::optional<std::string>& title, TTF_Font* font);

	void add_choice(const int choice_num, const std::string& str);
	void update_selected_choice_from_events(Events& events);
	void render_menu(const Window& window);
	int get_selected_choice() const;

	static void render_rect_and_pokeballs(const Window& window, SDL_Rect* pos_dst);

	const SDL_Color _text_color = { 0, 0, 0, 255 };
};