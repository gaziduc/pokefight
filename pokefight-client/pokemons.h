#pragma once

#include "anim.h"
#include <vector>
#include <string>
#include <utility>
#include <memory>

// Forward declaration
class Anim;

class Pokemons {
private:
	std::vector<std::pair<std::string, std::shared_ptr<Anim>>> _name_anim_pairs;

public:
	Pokemons();
	void add_pokemon(const std::string& pokemon_name, std::shared_ptr<Anim> anim_ptr);
	size_t get_num_pokemons() const;
	std::string get_pokemon_name(const size_t pokemon_num) const;
	std::shared_ptr<Anim> get_pokemon_anim_ptr(const size_t pokemon_num) const;
};