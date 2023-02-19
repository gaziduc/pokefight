#pragma once

#include "attack.h"
#include <string>
#include <optional>
#include <vector>

class Player {
private:
	int _initial_health;
	int _health;
	std::string _nickname;
	int _chosen_pokemon;
	bool _is_ready = false;
	std::optional<std::vector<std::string>> _current_turn_action = std::nullopt;
	bool _show_pokemon = true;
	bool _dead = false;

public:
	Player(const int health, const std::string& nickname);
	Player();
	Player(const int inital_health, const int health, const std::string& nickname, const int chosen_pokemon, const bool is_ready);

	int get_health() const;
	bool get_show_pokemon() const;
	void set_show_pokemon(const bool show_pokemon);
	int get_initial_health() const;
	void decrease_health(const int damage);
	bool get_is_dead() const;
	void set_is_dead(const bool dead);
	void set_initial_health(const int health);
	std::string get_nickname() const;
	void set_nickname(const std::string& nickname);
	int get_chosen_pokemon() const;
	void set_chosen_pokemon(const int chosen_pokemon);
	std::optional<std::vector<std::string>> get_current_turn_action() const;
	void set_current_turn_action(const std::optional<std::vector<std::string>>& current_turn_action);
	bool get_is_ready() const;
	void set_is_ready(const bool is_ready);
};