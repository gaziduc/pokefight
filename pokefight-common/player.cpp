#include "player.h"
#include <string>
#include <optional>
#include <vector>

// Server
Player::Player(const int health, const std::string& nickname) {
	_health = health;
	_nickname = nickname;
}

// Deafult
Player::Player() {
}


// Client
Player::Player(const std::string& nickname, const int chosen_pokemon, const bool is_ready) {
	_nickname = nickname;
	_chosen_pokemon = chosen_pokemon;
	_is_ready = is_ready;
}



int Player::get_health() const {
	return _health;
}

std::string Player::get_nickname() const {
	return _nickname;
}

void Player::set_nickname(const std::string& nickname) {
	_nickname = nickname;
}

void Player::decrease_health(const int damage) {
	_health -= damage;
	if (_health < 0)
		_health = 0;
}

std::optional<std::vector<std::string>> Player::get_current_turn_action() const {
	return _current_turn_action;
}


void Player::set_current_turn_action(const std::optional<std::vector<std::string>>& current_turn_action) {
	_current_turn_action = current_turn_action;
}

int Player::get_chosen_pokemon() const {
	return _chosen_pokemon;
}

void Player::set_chosen_pokemon(const int chosen_pokemon) {
	_chosen_pokemon = chosen_pokemon;
}

bool Player::get_is_ready() const {
	return _is_ready;
}

void Player::set_is_ready(const bool is_ready) {
	_is_ready = is_ready;
}
