
#include "utils.h"
#include "window.h"
#include "../pokefight-common/const.h"
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


std::vector<std::vector<std::string>> recv_network_splitted_msg_from_server_non_blocking(SDLNet_SocketSet socket_set, TCPsocket socket) {
	int sockets_ready_for_reading = SDLNet_CheckSockets(socket_set, 0);
	if (sockets_ready_for_reading == -1)
		throw std::runtime_error(SDLNet_GetError());

	std::vector<std::string> splitted_messages;
	if (sockets_ready_for_reading > 0) {
		char data[MAX_MESSAGE_SIZE] = { 0 };
		SDLNet_TCP_Recv(socket, data, MAX_MESSAGE_SIZE);
		std::string received_message(data);

		size_t start;
		size_t end = 0;
		
		while ((start = received_message.find_first_not_of(MESSAGE_DELIMITER, end)) != std::string::npos)
		{
			end = received_message.find(MESSAGE_DELIMITER, start);
			splitted_messages.push_back(received_message.substr(start, end - start));
		}
	}

	std::vector<std::vector<std::string>> splitted_messages_words;
	for (auto& message : splitted_messages) {
		std::vector<std::string> splitted_words;

		size_t start;
		size_t end = 0;

		while ((start = message.find_first_not_of(MESSAGE_WORD_DELIMITER, end)) != std::string::npos)
		{
			end = message.find(MESSAGE_WORD_DELIMITER, start);
			splitted_words.push_back(message.substr(start, end - start));
		}

		splitted_messages_words.push_back(splitted_words);
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