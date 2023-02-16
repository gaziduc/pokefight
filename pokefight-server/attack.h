#pragma once

#include "../pokefight-common/player.h"
#include <SDL2/SDL_net.h>
#include <string>
#include <map>
#include <vector>

std::string send_attack_to_clients(
	const std::string& attack_name,
	Player& player_to_attack,
	const std::string& attacking_player,
	const std::vector<std::pair<TCPsocket, Player>>& clients_map
);