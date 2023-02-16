#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string>

class IPv4 {
private:
	int _first_num;
	int _second_num;
	int _third_num;
	int _fourth_num;

public:
	IPv4(IPaddress* ip_address);

	std::string to_string() const;
};