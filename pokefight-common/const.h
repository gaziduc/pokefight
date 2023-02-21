#pragma once

#include "attack.h"
#include <map>
#include <vector>


constexpr int MAX_LOCAL_IPS_NUM = 15;
constexpr int MAX_MESSAGE_SIZE = 1024;
constexpr int MAX_CLIENTS_SOCKETS_NUM = 10;
constexpr char MESSAGE_DELIMITER = '\n';
constexpr char MESSAGE_WORD_DELIMITER = '@';

inline static const std::vector<std::tuple<int, Type, std::optional<Type>>> PKMN_STATS = {
		{ 310, Type::GRASS, Type::POISON }, // Venosaur
		{ 290, Type::FIRE, Type::FLYING }, // Charizard
		{ 300, Type::WATER, std::nullopt } // Balstoise
};

inline static const std::vector<std::vector<std::string>> PKMN_ATTACKS = {
		{ "Tackle", "Solar Beam", "Rock Smash" }, // Venosaur
		{ "Tackle", "Fire Blast", "Air Cutter" }, // Charizard
		{ "Tackle", "Hydro Pump", "Ice Beam" }  // Balstoise
};

inline static const std::map<std::string, Attack> ATTACK_STATS = {
		{ "Tackle", { 35, Type::NORMAL_TYPE }},
		{ "Fire Blast", { 110, Type::FIRE }},
		{ "Hydro Pump", { 110, Type::WATER }},
		{ "Solar Beam", { 110, Type::GRASS }},
		{ "Air Cutter", { 60, Type::FLYING }},
		{ "Ice Beam", { 90, Type::ICE }},
		{ "Rock Smash", { 40, Type::FIGHTING }}
};

inline static const float TYPE_ATTACK_MULTIPLICATOR_CHART[NUM_TYPES][NUM_TYPES] = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0, 1, 1, 0.5, 1 }, // NORMAL
		{ 1, 0.5, 0.5, 1, 2, 2, 1, 1, 1, 1, 1, 2, 0.5, 1, 0.5, 1, 2, 1 }, // FIRE
		{ 1, 2, 0.5, 1, 0.5, 1, 1, 1, 2, 1, 1, 1, 2, 1, 0.5, 1, 1, 1 },  // WATER
	    { 1, 1, 2, 0.5, 0.5, 1, 1, 1, 0, 2, 1, 1, 1, 1, 0.5, 1, 1, 1 },  // ELECTRIC
		{ 1, 0.5, 2, 1, 0.5, 1, 1, 0.5, 2, 0.5, 1, 0.5, 2, 1, 0.5, 1, 0.5, 1 }, // GRASS
		{ 1, 0.5, 0.5, 1, 2, 0.5, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 0.5, 1 }, // ICE
		{ 2, 1, 1, 1, 1, 2, 1, 0.5, 1, 0.5, 0.5, 0.5, 2, 0, 1, 2, 2, 0.5 }, // FIGHTING
		{ 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 1, 1, 0, 2 }, // POISON
		{ 1, 2, 1, 2, 0.5, 1, 1, 2, 1, 0, 1, 0.5, 2, 1, 1, 1, 2, 1 }, // GROUND
		{ 1, 1, 1, 0.5, 2, 1, 2, 1, 1, 1, 1, 2, 0.5, 1, 1, 1, 0.5, 1 }, // FLYING
		{ 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 0.5, 1, 1, 1, 1, 0, 0.5, 1 }, // PSYCHIC
		{ 1, 0.5, 1, 1, 2, 1, 0.5, 0.5, 1, 0.5, 2, 1, 1, 0.5, 1, 2, 0.5, 0.5 }, // BUG
		{ 1, 2, 1, 1, 1, 2, 0.5, 1, 0.5, 2, 1, 2, 1, 1, 1, 1, 0.5, 1 }, // ROCK
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 0.5, 1, 1 }, // GHOST
	    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0.5, 0 }, // DRAGON
		{ 1, 1, 1, 1, 1, 1, 0.5, 1, 1, 1, 2, 1, 1, 2, 1, 0.5, 1, 0.5 }, // DARK
		{ 1, 0.5, 0.5, 0.5, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0.5, 2 }, // STEEL
		{ 1, 0.5, 1, 1, 1, 1, 2, 0.5, 1, 1, 1, 1, 1, 1, 2, 2, 0.5, 1 } // FAIRY
};