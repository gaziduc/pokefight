#include "../pokefight-common/player.h"
#include <map>
#include <string>
#include <cstdlib>
#include <vector>
#include <SDL2/SDL_net.h>

std::string send_attack_to_clients(
	const std::string& attack_name,
    Player& player_to_attack,
	const std::string& attacking_player,
	const std::vector<std::pair<TCPsocket, Player>>& clients_map
) {
	std::string message_to_send = "TO_CLIENT_ATTACK " + attack_name + ' ' + player_to_attack.get_nickname() + ' ' + attacking_player + ' ';

	const bool is_critical = std::rand() % 10 == 0;
	const int thorical_damage = 20;
	const int final_damage = is_critical ? thorical_damage * 2 : thorical_damage;

	player_to_attack.decrease_health(final_damage);

	message_to_send += final_damage;
	return message_to_send;
}