#pragma once

#include "player.h"
#include <SDL2/SDL_net.h>
#include <string>
#include <map>
#include <vector>
#include <optional>

// Forward declaration
class Player;

enum Type {
	NORMAL_TYPE,
	FIRE,
	WATER,
	ELECTRIC,
	GRASS,
	ICE,
	FIGHTING,
	POISON,
	GROUND,
	FLYING,
	PSYCHIC,
	BUG,
	ROCK,
	GHOST,
	DRAGON,
	DARK,
	STEEL,
	FAIRY,
	NUM_TYPES
};


std::string send_attack_to_clients(
	const std::string& attack_name,
	Player& player_to_attack,
	const std::string& attacking_player,
	const std::vector<std::pair<TCPsocket, Player>>& clients_map
);


class Attack {
private:
	int _theorical_damage;
	Type _type;

public:
	Attack(const int theorical_damage, const Type type);

	int get_theorical_damage() const;
	Type get_type() const;
};