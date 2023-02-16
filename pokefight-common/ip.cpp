
#include "ip.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

IPv4::IPv4(IPaddress* ip_address) {
    _first_num = ip_address->host % 256;
    _second_num = (ip_address->host % 65536 - _first_num) / 256;
    _third_num = (ip_address->host % 16777216 - (_first_num + _second_num)) / 65536;
    _fourth_num = (ip_address->host % 4294967296 - (_first_num + _second_num + _third_num)) / 16777216;
}

std::string IPv4::to_string() const {
    return std::to_string(_first_num) + "." + std::to_string(_second_num) + "." + std::to_string(_third_num) + "." + std::to_string(_fourth_num);
}