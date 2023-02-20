
#pragma once

#include "pokemons.h"
#include "window.h"
#include <string>

class Pokemons;

enum SettingsMenu {
	NICKNAME = 0,
	POKEMON = 1,
	TOOGLE_FULLSCREEN = 2,
	BACK_TO_MAIN_MENU = 3
};

class Settings {
private:
	std::string _nickname;
	int _pokemon_num;
	bool _is_fullscreen;

public:
	Settings();

	void load_settings();
	void save_settings() const;

	std::string get_nickname() const;
	void set_nickname(const std::string& nickname);
	int get_pokemon_num() const;
	void set_pokemon_num(const int pokemon_num);
	bool get_is_fullscreen() const;
	void set_is_fullscreen(const bool is_fullscreen);
};