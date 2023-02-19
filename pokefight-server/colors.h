#pragma once

#include <string>

class Colors {
public:
	static void enable_colors();
	static std::string yellow();
	static std::string green();
	static std::string cyan();
	static std::string reset();
};