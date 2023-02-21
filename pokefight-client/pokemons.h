#pragma once

#include "anim.h"
#include "../pokefight-common/attack.h"
#include <vector>
#include <string>
#include <utility>
#include <memory>

// Forward declaration
class Anim;

class Pokemons {
private:
	std::vector<std::pair<std::string, std::shared_ptr<Anim>>> _name_anim_pairs;
	std::vector<std::shared_ptr<Anim>> _back_anims;
	std::vector<std::vector<std::string>> _attacks;

public:
	Pokemons();
	void add_pokemon(const std::string& pokemon_name, std::shared_ptr<Anim> anim_ptr, std::shared_ptr<Anim> back_anim_ptr);
	size_t get_num_pokemons() const;
	std::string get_pokemon_name(const size_t pokemon_num) const;
	std::shared_ptr<Anim> get_pokemon_anim_ptr(const size_t pokemon_num) const;
	std::shared_ptr<Anim> get_pokemon_back_anim_ptr(const size_t pokemon_num) const;
	int get_initial_health_points(const int pokemon_num) const;
	Type get_pokemon_first_type(const int pokemon_num) const;
	std::optional<Type> get_pokemon_second_type(const int pokemon_num) const;
};