#pragma once

#include "attack.h"
#include <map>
#include <vector>


constexpr int MAX_LOCAL_IPS_NUM = 15;
constexpr int MAX_MESSAGE_SIZE = 1024;
constexpr int MAX_CLIENTS_SOCKETS_NUM = 10;
constexpr char MESSAGE_DELIMITER = '\n';
constexpr char MESSAGE_WORD_DELIMITER = '@';

inline static const std::vector<std::pair<int, Type>> PKMN_STATS = {
		{ 310, Type::GRASS }, // Venosaur
		{ 290, Type::FIRE }, // Charizard
		{ 300, Type::WATER } // Balstoise
};

inline static const std::vector<std::vector<std::string>> PKMN_ATTACKS = {
		{ "Tackle", "Solar Beam" }, // Venosaur
		{ "Tackle", "Fire Blast" }, // Charizard
		{ "Tackle", "Hydro Pump" }  // Balstoise
};

inline static const std::map<std::string, Attack> ATTACK_STATS = {
		{ "Tackle", { 35, Type::NORMAL_TYPE }},
		{ "Fire Blast", { 120, Type::FIRE }},
		{ "Hydro Pump", { 120, Type::WATER }},
		{ "Solar Beam", { 120, Type::GRASS }}
};

inline static const float TYPE_ATTACK_MULTIPLICATOR_CHART[NUM_TYPES][NUM_TYPES] = {
		{ 1, 1, 1, 1 },
		{ 1, 0.5, 0.5, 2 },
		{ 1, 2, 0.5, 0.5 },
		{ 1, 0.5, 2, 0.5 }
};