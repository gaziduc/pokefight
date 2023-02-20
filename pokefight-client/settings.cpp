
#include "../pokefight-common/const.h"
#include "menu.h"
#include "settings.h"
#include "input.h"
#include "pokemons.h"
#include "utils.h"
#include "texture.h"
#include <string>
#include <fstream>
#include <algorithm>

Settings::Settings() {
}



void Settings::load_settings() {
	std::ifstream settings_file_read("settings.txt");

	if (settings_file_read.is_open()) {
		// Nickname
		std::getline(settings_file_read, _nickname);
		std::replace(_nickname.begin(), _nickname.end(), MESSAGE_WORD_DELIMITER, '_');

		// Chosen pokemon
		std::string pokemon;
		std::getline(settings_file_read, pokemon);

		try {
			_pokemon_num = std::stoi(pokemon);
		}
		catch (const std::exception& excetpion) {
			_pokemon_num = -1;
		}

		// Is fullscreen
		std::string is_fullscreen;
		std::getline(settings_file_read, is_fullscreen);

		try {
			_is_fullscreen = (bool)std::stoi(is_fullscreen);
		} catch (const std::exception& exception) {
			_is_fullscreen = true;
		}

		settings_file_read.close();
	}
	else {
		_pokemon_num = -1;
		_is_fullscreen = true;
	}
}


void Settings::save_settings() const {
	std::ofstream settings_file_write("settings.txt");

	if (settings_file_write.is_open()) {
		settings_file_write.clear();
		settings_file_write << _nickname << '\n';
		settings_file_write << _pokemon_num << '\n';
		settings_file_write << _is_fullscreen << '\n';
		settings_file_write.close();
	}
}

std::string Settings::get_nickname() const {
	return _nickname;
}

void Settings::set_nickname(const std::string& nickname) {
	_nickname = nickname;
}

int Settings::get_pokemon_num() const {
	return _pokemon_num;
}

void Settings::set_pokemon_num(const int pokemon_num) {
	_pokemon_num = pokemon_num;
}

bool Settings::get_is_fullscreen() const {
	return _is_fullscreen;
}

void Settings::set_is_fullscreen(const bool is_fullscreen) {
	_is_fullscreen = is_fullscreen;
}
	