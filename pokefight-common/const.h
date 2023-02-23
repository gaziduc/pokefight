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
	{ 300, Type::WATER, std::nullopt }, // Balstoise
	{ 250, Type::ELECTRIC, std::nullopt }, // Raichu
	{ 330, Type::PSYCHIC, std::nullopt }, // Mewtwo
};

inline static const std::vector<std::vector<std::string>> PKMN_ATTACKS = {
	{ "Tackle", "Solar Beam", "Rock Smash", "Sludge Bomb" }, // Venosaur
	{ "Tackle", "Fire Blast", "Air Cutter", "Flamethrower" }, // Charizard
	{ "Tackle", "Hydro Pump", "Ice Beam", "Rock Slide" },  // Balstoise
	{ "Thunder", "Surf", "Facade", "Take Down" }, // Raichu
	{ "Future Sight", "Psychic", "Blizzard", "Shadow Ball" } // Mewtwo
};

inline static const std::map<std::string, Attack> ATTACK_STATS = {
	{ "Tackle", { 35, Type::NORMAL_TYPE }},
	{ "Fire Blast", { 110, Type::FIRE }},
	{ "Hydro Pump", { 110, Type::WATER }},
	{ "Solar Beam", { 110, Type::GRASS }},
	{ "Air Cutter", { 60, Type::FLYING }},
	{ "Ice Beam", { 90, Type::ICE }},
	{ "Rock Smash", { 40, Type::FIGHTING }},
	{ "Sludge Bomb", { 90, Type::POISON }},
	{ "Flamethrower", { 90, Type::FIRE }},
	{ "Rock Slide", { 75, Type::ROCK }},
	{ "Thunder", { 110, Type::ELECTRIC }},
	{ "Surf", { 90, Type::WATER }},
	{ "Facade", { 70, Type::NORMAL_TYPE }},
	{ "Take Down", { 90, Type::NORMAL_TYPE }},
	{ "Future Sight", { 120, Type::PSYCHIC }},
	{ "Psychic", { 90, Type::PSYCHIC }},
	{ "Blizzard", { 110, Type::ICE }},
	{ "Shadow Ball", { 80, Type::GHOST }}
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