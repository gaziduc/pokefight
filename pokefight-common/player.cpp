#include "player.h"
#include <string>
#include <optional>
#include <vector>

// Server
Player::Player(const int health, const std::string& nickname) {
	_initial_health = health;
	_health = health;
	_nickname = nickname;
}

// Default
Player::Player() {
}


// Client
Player::Player(const int inital_health, const int health, const std::string& nickname, const int chosen_pokemon, const bool is_ready) {
	_nickname = nickname;
	_chosen_pokemon = chosen_pokemon;
	_is_ready = is_ready;
	_health = health;
	_initial_health = inital_health;
}

bool Player::get_show_pokemon() const {
	return _show_pokemon;
}

void Player::set_show_pokemon(const bool show_pokemon) {
	_show_pokemon = show_pokemon;
}

int Player::get_health() const {
	return _health;
}

int Player::get_initial_health() const {
	return _initial_health;
}

void Player::decrease_health(const int damage) {
	_health -= damage;
	if (_health < 0)
		_health = 0;
	if (_health == 0)
		set_is_dead(true);
}


bool Player::get_is_dead() const {
	return _dead;
}

void Player::set_is_dead(const bool dead) {
	_dead = dead;
}

void Player::set_initial_health(const int health) {
	_initial_health = health;
	_health = health;
}


std::string Player::get_nickname() const {
	return _nickname;
}

void Player::set_nickname(const std::string& nickname) {
	_nickname = nickname;
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

void Player::reset_player() {
	_is_ready = false;
	_health = _initial_health;
	_dead = false;
	_current_turn_action = std::nullopt;
	_show_pokemon = true;
}