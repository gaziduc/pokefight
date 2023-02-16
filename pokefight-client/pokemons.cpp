#include "pokemons.h"
#include "anim.h"
#include <string>
#include <memory>

Pokemons::Pokemons() {

}

void Pokemons::add_pokemon(const std::string& pokemon_name, std::shared_ptr<Anim> anim_ptr) {
	_name_anim_pairs.emplace_back(pokemon_name, anim_ptr);
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