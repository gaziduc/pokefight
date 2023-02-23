#pragma once

#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <string>
#include <vector>

SDL_Texture* get_text_texture(const Window& window, TTF_Font* font, const std::string& text, SDL_Color text_color);
SDL_Texture* get_text_texture(const Window& window, TTF_Font* font, const std::string& text, SDL_Color text_color, const bool is_utf8);
std::vector<std::vector<std::string>> recv_network_splitted_msg_from_server_non_blocking(Window& window, SDLNet_SocketSet socket_set, TCPsocket socket);
void pop_back_utf8(std::string& utf8_str);
std::string utf8_substr(const std::string& str, unsigned int start, unsigned int leng);