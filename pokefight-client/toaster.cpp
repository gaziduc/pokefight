#include "toaster.h"
#include "utils.h"
#include "window.h"
#include <SDL2/SDL.h>

Toaster::Toaster(const std::string& message, const int time_left) {
	_message = message;
	_time_left = time_left;
}

std::string Toaster::get_message() const {
	return _message;
}

int Toaster::get_time_left() const {
	return _time_left;
}

void Toaster::decrease_time_left() {
	_time_left -= 16;
}