#include "const.h"
#include <vector>
#include <string>
#include <SDL2/SDL_net.h>
#include <stdexcept>


std::vector<std::vector<std::string>> recv_network_splitted_msg_non_blocking(SDLNet_SocketSet socket_set, TCPsocket socket) {
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