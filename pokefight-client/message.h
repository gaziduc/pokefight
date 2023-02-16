#pragma once

#include "window.h"
#include <string>

class Message {
public:
	static void show_message(Window& window, const std::string& message, const bool is_utf8);
};