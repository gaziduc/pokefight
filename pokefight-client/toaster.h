#pragma once

#include "window.h"

class Toaster {
private:
	std::string _message;
	int _time_left;

public:
	Toaster(const std::string& message, const int time_left);

	std::string get_message() const;
	int get_time_left() const;

	void decrease_time_left();
};