
#include "utils.h"
#include "window.h"
#include "../pokefight-common/const.h"
#include "../pokefight-common/utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <vector>
#include <string>
#include <stdexcept>

SDL_Texture* get_text_texture(const Window& window, TTF_Font* font, const std::string& text, SDL_Color text_color) {
	return get_text_texture(window, font, text, text_color, false);
}

SDL_Texture* get_text_texture(const Window& window, TTF_Font* font, const std::string& text, SDL_Color text_color, const bool is_utf8) {
	SDL_Surface* surface = nullptr;
	if (is_utf8)
		surface = TTF_RenderUTF8_Blended(font, text.c_str(), text_color);
	else
		surface = TTF_RenderText_Blended(font, text.c_str(), text_color);

	if (surface == nullptr)
		throw std::runtime_error("Error while creating text surface:\n" + std::string(TTF_GetError()));

	SDL_Texture* texture = SDL_CreateTextureFromSurface(window.get_renderer(), surface);
	if (texture == nullptr)
		throw std::runtime_error("Error while creating text texture:\n" + std::string(SDL_GetError()));

	SDL_FreeSurface(surface);

	return texture;
}

std::vector<std::vector<std::string>> recv_network_splitted_msg_from_server_non_blocking(Window& window, SDLNet_SocketSet socket_set, TCPsocket socket) {

	auto splitted_messages_words = recv_network_splitted_msg_non_blocking(socket_set, socket);

	for (const std::vector<std::string>& message : splitted_messages_words) {
		if (message[0] == "TO_CLIENT_DISCONNECTED_PLAYER") {
			window.add_toaster(message[1] + " disconnected.");
		}
	}

	return splitted_messages_words;
}

void pop_back_utf8(std::string& utf8_str)
{
	if (utf8_str.empty())
		return;

	auto cp = utf8_str.data() + utf8_str.size();
	while (--cp >= utf8_str.data() && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {}
	if (cp >= utf8_str.data())
		utf8_str.resize(cp - utf8_str.data());
}

std::string utf8_substr(const std::string& str, unsigned int start, unsigned int leng)
{
	if (leng == 0)
		return "";

	unsigned int c, i, ix, q, min = std::string::npos, max = std::string::npos;
	for (q = 0, i = 0, ix = str.length(); i < ix; i++, q++)
	{
		if (q == start)
			min = i;
		if (q <= start + leng || leng == std::string::npos)
			max = i;

		c = (unsigned char) str[i];
		if (c <= 127)
			i += 0;
		else if ((c & 0xE0) == 0xC0)
			i += 1;
		else if ((c & 0xF0) == 0xE0)
			i += 2;
		else if ((c & 0xF8) == 0xF0)
			i += 3;
		else
			return "";
	}

	if (q <= start + leng || leng == std::string::npos)
		max = i;

	if (min == std::string::npos || max == std::string::npos)
		return "";

	return str.substr(min, max);
}
