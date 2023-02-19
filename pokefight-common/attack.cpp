#include "attack.h"
#include "const.h"
#include "player.h"
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
	std::string message_to_send = "TO_CLIENT_ATTACK";
	message_to_send += MESSAGE_WORD_DELIMITER + attack_name + MESSAGE_WORD_DELIMITER + player_to_attack.get_nickname() + MESSAGE_WORD_DELIMITER + attacking_player + MESSAGE_WORD_DELIMITER;

	const bool is_critical = std::rand() % 10 == 0;

	const int damage = ATTACK_STATS.at(attack_name).get_damage(PKMN_STATS[player_to_attack.get_chosen_pokemon()].second);
	const int final_damage = is_critical ? damage * 2 : damage;

	player_to_attack.decrease_health(final_damage);

	message_to_send += std::to_string(final_damage) + MESSAGE_WORD_DELIMITER + std::to_string(is_critical) + MESSAGE_DELIMITER;
	return message_to_send;
}

Attack::Attack(const int theorical_damage, const Type type) {
	_theorical_damage = theorical_damage;
	_type = type;
}

int Attack::get_theorical_damage() const {
	return _theorical_damage;
}

Type Attack::get_type() const {
	return _type;
}


int Attack::get_damage(const Type enemy_type) const {
	return _theorical_damage * TYPE_ATTACK_MULTIPLICATOR_CHART[_type][enemy_type];
}