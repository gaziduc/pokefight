#pragma once

#include "window.h"
#include <string>

class Message {
public:
	static void show_message_battle(Window& window, const std::string& message, const bool is_utf8, const Pokemons& pokemons, const std::shared_ptr<Player>& my_player_ptr, const std::vector<std::shared_ptr<Player>>& other_players_ptr);
};