#pragma once

#include "window.h"

class Hud {
public:
	static void show_pokemon_hud(Window& window, const std::string& nickname, const int initial_health, const int current_health, const int pos_dst_x, const int pos_dst_y);

private:
	static void show_health(Window& window, const int initial_health, const int current_health, const int pos_dst_x, const int pos_dst_y);
};