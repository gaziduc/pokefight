
#pragma once

#include "pokemons.h"
#include "window.h"
#include <string>

class Pokemons;

enum SettingsMenu {
	NICKNAME = 0,
	POKEMON = 1,
	BACK_TO_MAIN_MENU = 2
};

class Settings {
private:
	std::string _nickname;
	int _pokemon_num;

public:
	Settings();

	void load_settings();
	void save_settings() const;

	std::string get_nickname() const;
	void set_nickname(const std::string& nickname);
	int get_pokemon_num() const;
	void set_pokemon_num(const int pokemon_num);
};