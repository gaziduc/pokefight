#include "pokemons.h"
#include "anim.h"
#include "../pokefight-common/const.h"
#include <string>
#include <memory>

Pokemons::Pokemons() {

}

void Pokemons::add_pokemon(const std::string& pokemon_name, std::shared_ptr<Anim> anim_ptr, std::shared_ptr<Anim> back_anim_ptr) {
	_attacks.emplace_back(PKMN_ATTACKS[_attacks.size()]);
	_name_anim_pairs.emplace_back(pokemon_name, anim_ptr);
	_back_anims.emplace_back(back_anim_ptr);
}

size_t Pokemons::get_num_pokemons() const {
	return _name_anim_pairs.size();
}

std::string Pokemons::get_pokemon_name(const size_t pokemon_num) const {
	return _name_anim_pairs[pokemon_num].first;
}

std::shared_ptr<Anim> Pokemons::get_pokemon_anim_ptr(const size_t pokemon_num) const {
	return _name_anim_pairs[pokemon_num].second;
}

std::shared_ptr<Anim> Pokemons::get_pokemon_back_anim_ptr(const size_t pokemon_num) const {
	return _back_anims[pokemon_num];
}

int Pokemons::get_initial_health_points(const int pokemon_num) const {
	return PKMN_STATS[pokemon_num].first;
}

Type Pokemons::get_pokemon_type(const int pokemon_num) const {
	return PKMN_STATS[pokemon_num].second;
}