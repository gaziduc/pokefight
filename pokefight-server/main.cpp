#include "../pokefight-common/ip.h"
#include "../pokefight-common/player.h"
#include "../pokefight-common/const.h"
#include "../pokefight-common/attack.h"
#include "../pokefight-common/utils.h"
#include "colors.h"
#include <ctime>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#ifdef _WIN32
#include <windows.h>
#endif

void send_infos_to_clients(std::vector<std::pair<TCPsocket, Player>>& clients_list, std::optional<std::string> disconnected_player);

int main(int argc, char* argv[]) {
	Colors::enable_colors();
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif
	std::cout << Colors::yellow() << "Welcome to Pokefight server!" << Colors::reset() << '\n';

	if (SDLNet_Init() == -1) {
		std::cout << "Error: " << SDLNet_GetError() << '\n';
		return 1;
	}

	int port = -1;
	while (port == -1) {
		std::cout << "Enter server port: ";
		std::cin >> port;
	}

	IPaddress server_address;
	if (SDLNet_ResolveHost(&server_address, nullptr, port) == -1) {
		std::cout << "Error: " << SDLNet_GetError() << '\n';
		return 1;
	}
	
	TCPsocket server_socket = SDLNet_TCP_Open(&server_address);
	if (server_socket == nullptr) {
		std::cout << "Error: " << SDLNet_GetError() << '\n';
		return 1;
	}
	
	IPaddress local_ips[MAX_LOCAL_IPS_NUM];
	const int num_local_ips = SDLNet_GetLocalAddresses(local_ips, MAX_LOCAL_IPS_NUM);

	std::cout << "Clients on your local network can join you with:\n";

	for (int local_ip_num = 0; local_ip_num < num_local_ips; local_ip_num++) {
		IPaddress curr_local_ip = local_ips[local_ip_num];
		if (curr_local_ip.host != 0) {
			IPv4 curr_ipv4(&curr_local_ip);
			std::cout << Colors::yellow() << curr_ipv4.to_string() << Colors::reset() << ':' << Colors::green() << port << Colors::reset() << " (" << SDLNet_ResolveIP(&curr_local_ip) << ")\n";
		}
	}

	std::vector<std::pair<TCPsocket, Player>> client_list;
	SDLNet_SocketSet socket_set = SDLNet_AllocSocketSet(MAX_CLIENTS_SOCKETS_NUM);

	// Use current time as seed for random generator
	std::srand(std::time(nullptr));

	std::cout << "Server is now listening...\n";
	
	while (true) {
		TCPsocket potential_new_client_socket = SDLNet_TCP_Accept(server_socket);

		// Accept new connections
		if (potential_new_client_socket != nullptr) {
			IPaddress* new_client_ip_address = SDLNet_TCP_GetPeerAddress(potential_new_client_socket);
			IPv4 curr_ipv4(new_client_ip_address);

			std::cout << "User at " << Colors::yellow() << curr_ipv4.to_string() << Colors::reset() << ':' << Colors::green() << new_client_ip_address->port << Colors::reset() << " (" << SDLNet_ResolveIP(new_client_ip_address) << ") joined the server!\n";
			
			std::string ip_and_port = curr_ipv4.to_string() + ':' + std::to_string(new_client_ip_address->port);
			Player new_player(100, ip_and_port);
			client_list.push_back(std::make_pair<>(potential_new_client_socket, new_player));
			SDLNet_TCP_AddSocket(socket_set, potential_new_client_socket);
		}

		if (!client_list.empty()) {
			int sockets_ready_for_reading = SDLNet_CheckSockets(socket_set, 0);
			if (sockets_ready_for_reading == -1)
				std::cout << "Error: " << SDLNet_GetError() << '\n';
			else if (sockets_ready_for_reading > 0) {
				for (auto socket_iter = client_list.begin(); socket_iter != client_list.end();) {
					TCPsocket curr_socket = socket_iter->first;
					Player& player = socket_iter->second;

					if (SDLNet_SocketReady(curr_socket) > 0) {
						char data[MAX_MESSAGE_SIZE] = { 0 };
						if (SDLNet_TCP_Recv(curr_socket, data, MAX_MESSAGE_SIZE) <= 0) {
							IPaddress* new_client_ip_address = SDLNet_TCP_GetPeerAddress(curr_socket);
							IPv4 curr_ipv4(new_client_ip_address);

							std::string disconnected_nickname = socket_iter->second.get_nickname();
							std::cout << "User " << Colors::cyan() << disconnected_nickname << Colors::reset() << " at " << Colors::yellow() << curr_ipv4.to_string() << Colors::reset() << ':' << Colors::green() << new_client_ip_address->port << Colors::reset() << " (" << SDLNet_ResolveIP(new_client_ip_address) << ") disconnected.\n";

							// Remove socket and close connection
							SDLNet_TCP_DelSocket(socket_set, curr_socket);
							socket_iter = client_list.erase(socket_iter);
							SDLNet_TCP_Close(curr_socket);

							send_infos_to_clients(client_list, disconnected_nickname);

							continue;
						}
						
						std::string received_message(data);

						std::vector<std::string> splitted_messages;

						size_t start;
						size_t end = 0;

						while ((start = received_message.find_first_not_of(MESSAGE_DELIMITER, end)) != std::string::npos)
						{
							end = received_message.find(MESSAGE_DELIMITER, start);
							splitted_messages.push_back(received_message.substr(start, end - start));
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


						for (auto& message : splitted_messages_words) {
							if (message[0] == "TO_SERVER_PLAYER_INFOS") {
								socket_iter->second.set_nickname(message[1]);

								try {
									int chosen_pokemon = std::stoi(message[2]);
									socket_iter->second.set_chosen_pokemon(chosen_pokemon);

									int health = std::stoi(message[3]);
									socket_iter->second.set_initial_health(health);
								}
								catch (const std::exception& exception) {

								}

								send_infos_to_clients(client_list, std::nullopt);
							}
							else if (message[0] == "TO_SERVER_READINESS") {
								for (auto& client : client_list) {
									Player& player = client.second;
									if (player.get_nickname() == message[1]) {
										player.set_is_ready((bool)std::stoi(message[2]));
										break;
									}
								}

								send_infos_to_clients(client_list, std::nullopt);
							}
							else if (message[0] == "TO_SERVER_ATTACK") {
								player.set_current_turn_action(message);
							}
						}
					}

					socket_iter++;
				}

				bool play_next_turn = true;

				for (auto& client : client_list) {
					Player& player = client.second;
					if (!player.get_current_turn_action().has_value() && !player.get_is_dead()) {
						play_next_turn = false;
						break;
					}
				}
				
				if (play_next_turn) {
					std::string message_to_send;

					for (auto& client : client_list) {
						Player& player = client.second;
						if (!player.get_is_dead()) {
							std::vector<std::string> splitted_message = player.get_current_turn_action().value();
							std::string attack_name = splitted_message[1];
							std::string nickname_to_attack = splitted_message[2];

							for (auto& potential_attacked_pair : client_list) {
								Player& potential_attacked_player = potential_attacked_pair.second;
								if (potential_attacked_player.get_nickname() == nickname_to_attack) {
									message_to_send += send_attack_to_clients(attack_name, potential_attacked_player, player.get_nickname(), client_list);
									message_to_send += MESSAGE_DELIMITER;
								}
							}
						}
						
						player.set_current_turn_action(std::nullopt);
					}

					std::cout << message_to_send;
					for (auto& client : client_list) {
						SDLNet_TCP_Send(client.first, message_to_send.c_str(), message_to_send.size());
					}
				}
			}
		}

		SDL_Delay(1);
	}

	SDLNet_FreeSocketSet(socket_set);
	SDLNet_TCP_Close(server_socket);

	SDLNet_Quit();

	return 0;
}


void send_infos_to_clients(std::vector<std::pair<TCPsocket, Player>>& clients_list, std::optional<std::string> disconected_player) {
	std::string message = "TO_CLIENT_PLAYER_INFOS";
	for (auto& client : clients_list) {
		auto player = client.second;
		message += MESSAGE_WORD_DELIMITER + player.get_nickname() + MESSAGE_WORD_DELIMITER + std::to_string(player.get_chosen_pokemon()) + MESSAGE_WORD_DELIMITER + std::to_string(player.get_is_ready()) + MESSAGE_WORD_DELIMITER + std::to_string(player.get_health()) + MESSAGE_WORD_DELIMITER + std::to_string(player.get_initial_health());
	}

	message += MESSAGE_DELIMITER;

	// Send to all clients
	for (auto& client : clients_list) {
		SDLNet_TCP_Send(client.first, message.c_str(), message.size());
	}

	if (disconected_player.has_value()) {
		std::string disconnected_message = "TO_CLIENT_DISCONNECTED_PLAYER";
		disconnected_message += MESSAGE_WORD_DELIMITER + disconected_player.value() + MESSAGE_DELIMITER;

		for (auto& client : clients_list) {
			SDLNet_TCP_Send(client.first, disconnected_message.c_str(), disconnected_message.size());
		}
	}
}