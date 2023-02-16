#pragma once

#include <string>
#include <optional>
#include <vector>

class Player {
private:
	int _health;
	std::string _nickname;
	int _chosen_pokemon;
	bool _is_ready = false;
	std::optional<std::vector<std::string>> _current_turn_action = std::nullopt;

public:
	Player(const int health, const std::string& nickname);
	Player();
	Player(const std::string& nickname, const int chosen_pokemon, const bool is_ready);

	int get_health() const;
	std::string get_nickname() const;
	void set_nickname(const std::string& nickname);
	int get_chosen_pokemon() const;
	void set_chosen_pokemon(const int chosen_pokemon);
	void decrease_health(const int damage);
	std::optional<std::vector<std::string>> get_current_turn_action() const;
	void set_current_turn_action(const std::optional<std::vector<std::string>>& current_turn_action);
	bool get_is_ready() const;
	void set_is_ready(const bool is_ready);
};