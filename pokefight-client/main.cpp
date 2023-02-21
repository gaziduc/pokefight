// main.cpp : Defines the entry point for the application.
//

#include "main.h"
#include "window.h"
#include "anim.h"
#include "events.h"
#include "menu.h"
#include "texture.h"
#include "input.h"
#include "utils.h"
#include "message.h"
#include "pokemons.h"
#include "hud.h"
#include "../pokefight-common/ip.h"
#include "../pokefight-common/player.h"
#include "../pokefight-common/const.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include <optional>


int main(int argc, char *argv[])
{
	Window window(1280, 720);

	try {
		window.init();

		Pokemons pokemon_list;
		pokemon_list.add_pokemon("Venusaur", std::make_shared<Anim>(window, "data/anims/front/ani_bw_003_m.gif"), std::make_shared<Anim>(window, "data/anims/back/a-b_bw_003_m.gif"));
		pokemon_list.add_pokemon("Charizard", std::make_shared<Anim>(window, "data/anims/front/ani_bw_006.gif"), std::make_shared<Anim>(window, "data/anims/back/a-b_bw_006.gif"));
		pokemon_list.add_pokemon("Blastoise", std::make_shared<Anim>(window, "data/anims/front/ani_bw_009.gif"), std::make_shared<Anim>(window, "data/anims/back/a-b_bw_009.gif"));

		Menu menu(std::nullopt, window.get_font(FontSize::NORMAL), 600);
		menu.add_choice(MainMenu::FIGHT_ONLINE_PLAYERS, "Fight online players");
		menu.add_choice(MainMenu::FIGHT_AGAINST_COMPUTER, "Fight against computer");
		menu.add_choice(MainMenu::SETTINGS, "Settings");
		menu.add_choice(MainMenu::QUIT, "Quit");

		bool has_to_save = window.ask_nickname_if_necessary(false);
		has_to_save = window.ask_pokemon_if_necessary(false, pokemon_list) || has_to_save;
		if (has_to_save)
			window.get_settings().save_settings();

		while (true) {
			window.update_events();

			if (window.is_quit())
				break;

			if (window.is_enter_down()) {
				window.press_up_key(SDL_SCANCODE_RETURN);
				window.press_up_key(SDL_SCANCODE_KP_ENTER);

				switch (menu.get_selected_choice_num()) {
				case MainMenu::FIGHT_ONLINE_PLAYERS:
				{
					std::string error;
					do {
						error = connect(window, pokemon_list);
						if (!error.empty()) {
							Menu error_menu(error, window.get_font(FontSize::NORMAL), 1000);
							error_menu.add_choice(0, "Retry");
							error_menu.add_choice(1, "Back to main menu...");

							while (true) {
								window.update_events();

								if (window.is_quit())
									exit(0);

								error_menu.update_selected_choice_from_events(window.get_events());

								if (window.is_enter_down()) {
									window.press_up_key(SDL_SCANCODE_RETURN);
									window.press_up_key(SDL_SCANCODE_KP_ENTER);

									if (error_menu.get_selected_choice_num() == 1) {
										error.clear();
									}

									break;
								}

								window.render_clear();
								window.get_texture(Picture::MENU_BACKGROUND)->render_without_pos_dst(window);
								window.get_texture(Picture::TITLE)->set_pos_dst(window.get_width() / 2 - window.get_texture(Picture::TITLE)->get_width() / 2, 80);
								window.get_texture(Picture::TITLE)->render(window);
								error_menu.render_menu(window);
								window.render_present();
							}
						}
					} while (!error.empty());
					break;
				}	
				case MainMenu::FIGHT_AGAINST_COMPUTER:
					break;
				case MainMenu::SETTINGS:
					window.settings_menu(pokemon_list);
					break;
				case MainMenu::QUIT:
					exit(0);

				default:
					break;
				}
			}

			menu.update_selected_choice_from_events(window.get_events());

			window.render_clear();
			window.get_texture(Picture::MENU_BACKGROUND)->render_without_pos_dst(window);
			window.get_texture(Picture::TITLE)->set_pos_dst(window.get_width() / 2 - window.get_texture(Picture::TITLE)->get_width() / 2, 80);
			window.get_texture(Picture::TITLE)->render(window);
			std::shared_ptr<Anim> chosed_anim_ptr = pokemon_list.get_pokemon_anim_ptr(window.get_settings().get_pokemon_num());
			chosed_anim_ptr->set_pos_dst(window.get_width() / 2 - chosed_anim_ptr->get_width() / 2, window.get_height() / 2 - chosed_anim_ptr->get_height() / 2 - 50);
			chosed_anim_ptr->render_anim(window, true);
			menu.render_menu(window);
			window.render_present();
		}
	}
	catch (const std::runtime_error& runtime_exception) {
		window.close_window();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error while running pokefight", runtime_exception.what(), nullptr);
		exit(1);
	}

	return 0;
}

std::string connect(Window& window, const Pokemons& pokemons) {
	Input input_ip("Enter server hostname or IP address:", window.get_font(FontSize::NORMAL), "", false);
	if (input_ip.show_input_menu(window) == InputEndAction::ESCAPE)
		return "";

	if (input_ip.get_current_input().empty())
		return "Error: entered IP/hostname is empty.";

	Input input_port("Enter server port:", window.get_font(FontSize::NORMAL), "", false);
	if (input_port.show_input_menu(window) == InputEndAction::ESCAPE) {
		return "";
	}

	if (input_port.get_current_input().empty())
		return "Error: entered port is empty.";

	IPaddress ip;
	int port = 0;
	std::string error;
	try {
		port = std::stoi(input_port.get_current_input());
	}
	catch (const std::exception& exception) {
		return "Error: entered port " + input_port.get_current_input() + " is not a number.";
	}
	
	if (SDLNet_ResolveHost(&ip, input_ip.get_current_input().c_str(), port) == -1) {
		return "Error: " + input_ip.get_current_input() + ":" + std::to_string(port) + ": " + SDLNet_GetError();
	}

	TCPsocket socket = SDLNet_TCP_Open(&ip);
	if (socket == nullptr) {
		return "Error: " + input_ip.get_current_input() + ":" + std::to_string(port) + ": " + SDLNet_GetError();
	}

	// Send nickname and chosen pokemon to server
	std::string nickname_message("TO_SERVER_PLAYER_INFOS");
	nickname_message += MESSAGE_WORD_DELIMITER + window.get_settings().get_nickname() + MESSAGE_WORD_DELIMITER + std::to_string(window.get_settings().get_pokemon_num()) + MESSAGE_WORD_DELIMITER + std::to_string(pokemons.get_initial_health_points(window.get_settings().get_pokemon_num())) + MESSAGE_DELIMITER;
	SDLNet_TCP_Send(socket, nickname_message.c_str(), nickname_message.size());

	SDLNet_SocketSet socket_set = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socket_set, socket);
	
	std::vector<Player> players;
	while (wait_for_players_readiness(window, socket, socket_set, players, pokemons, input_ip.get_current_input() + ":" + std::to_string(port))) {
		battle(window, socket, socket_set, players, pokemons);

		for (auto& player : players) {
			player.reset_player();
		}

		window.load_and_play_music("data/route-203.ogg");
	}


	SDLNet_TCP_Close(socket);
	return "";
}

bool wait_for_players_readiness(Window& window, TCPsocket socket, SDLNet_SocketSet socket_set, std::vector<Player>& player_infos, const Pokemons& pokemons, const std::string& server_ip_port) {
	SDL_Color text_color = { 0, 0, 0, 255 };
	SDL_Color ready_text_color = { 36, 189, 81, 255 };
	bool is_ready = false;

	Uint64 last_ticks = SDL_GetTicks64();
	int alpha = 255;
	bool alpha_increase = false;

	while (true) {
		window.update_events();

		if (window.is_quit())
			exit(0);

		if (window.is_escape_down()) {
			window.press_up_key(SDL_SCANCODE_ESCAPE);

			return false;
		}

		if (window.is_enter_down()) {
			window.press_up_key(SDL_SCANCODE_RETURN);
			window.press_up_key(SDL_SCANCODE_KP_ENTER);

			is_ready = !is_ready;
			std::string readiness_message("TO_SERVER_READINESS");
			readiness_message += MESSAGE_WORD_DELIMITER + window.get_settings().get_nickname() + MESSAGE_WORD_DELIMITER + std::to_string(is_ready) + MESSAGE_DELIMITER;
			SDLNet_TCP_Send(socket, readiness_message.c_str(), readiness_message.size());
		}

		std::vector<std::vector<std::string>> splitted_message = recv_network_splitted_msg_from_server_non_blocking(window, socket_set, socket);
		if (!splitted_message.empty()) {
			for (auto& message : splitted_message) {
				if (message[0] == "TO_CLIENT_PLAYER_INFOS") {
					player_infos.clear();
					for (int i = 1; i < message.size(); i += 5) {
						player_infos.emplace_back(std::stoi(message[i + 4]), std::stoi(message[i + 3]), message[i], std::stoi(message[i + 1]), (bool)std::stoi(message[i + 2]));
					}
				}
				else if (message[0] == "TO_CLIENT_START_GAME") {
					return true;
				}
			}
		}

		window.render_clear();
		window.get_texture(Picture::MENU_BACKGROUND)->render_without_pos_dst(window);

		SDL_Rect pos_dst = { .x = 50, .y = 50, .w = 650, .h = 81 };
		Menu::render_rect_and_pokeballs(window, &pos_dst);
		SDL_Texture* server_ip_port_texture = get_text_texture(window, window.get_font(FontSize::SMALL), "Connected to: " + server_ip_port, text_color);
		SDL_QueryTexture(server_ip_port_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
		pos_dst.y += 80 / 2 - pos_dst.h / 2;
		pos_dst.x += 650 / 2 - pos_dst.w / 2;
		SDL_RenderCopy(window.get_renderer(), server_ip_port_texture, nullptr, &pos_dst);
		SDL_DestroyTexture(server_ip_port_texture);

		pos_dst.x = 50;
		pos_dst.y = 130;
		pos_dst.w = 650;
		pos_dst.h = window.get_height() - 260;
		Menu::render_rect_and_pokeballs(window, &pos_dst);

		pos_dst.x += 50;
		pos_dst.y += 30;

		for (Player& player_info : player_infos) {
			// Text (nickname)
			SDL_Texture* nickname_texture = get_text_texture(window, window.get_font(FontSize::NORMAL), player_info.get_nickname(), text_color, true);
			SDL_QueryTexture(nickname_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
			SDL_RenderCopy(window.get_renderer(), nickname_texture, nullptr, &pos_dst);
			SDL_DestroyTexture(nickname_texture);

			const int pos_nickname_x_right = pos_dst.x + pos_dst.w;
			
			pos_dst.y += 40;

			// Readiness
			Uint64 ticks = SDL_GetTicks64();
			SDL_Color not_ready_text_color = { 235, 51, 36, alpha };
			Uint64 diff = ticks - last_ticks;
			if (alpha_increase) {
				alpha += diff / 2;
				if (alpha >= 255) {
					alpha = 255;
					alpha_increase = false;
				}
			}
			else {
				alpha -= diff / 2;
				if (alpha <= 1) {
					alpha = 1;
					alpha_increase = true;
				}
			}

			last_ticks = ticks;



			const bool player_is_ready = player_info.get_is_ready();
			SDL_Texture* ready_texture = get_text_texture(window, window.get_font(FontSize::SMALL), player_is_ready ? "Ready!" : "Not ready...", player_is_ready ? ready_text_color : not_ready_text_color);
			SDL_QueryTexture(ready_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
			SDL_RenderCopy(window.get_renderer(), ready_texture, nullptr, &pos_dst);
			SDL_DestroyTexture(ready_texture);

			// Pokemon
			std::shared_ptr<Anim> anim_ptr = pokemons.get_pokemon_anim_ptr(player_info.get_chosen_pokemon());
			int pos_pkmn_dst_x = pos_dst.x + pos_dst.w + 50;
			if (pos_pkmn_dst_x < pos_nickname_x_right + 50)
				pos_pkmn_dst_x = pos_nickname_x_right + 50;
			anim_ptr->set_pos_dst(pos_pkmn_dst_x, pos_dst.y + pos_dst.h / 2 - anim_ptr->get_height() / 2 - 20);
			anim_ptr->render_anim(window, false);

			pos_dst.y += 65;
		}

		pos_dst.x = 50;
		pos_dst.y = window.get_height() - 50;

		SDL_Texture* escape_texture = get_text_texture(window, window.get_font(FontSize::SMALL), "Press 'Escape' to leave...", text_color);
		SDL_QueryTexture(escape_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
		pos_dst.y -= pos_dst.h;
		SDL_RenderCopy(window.get_renderer(), escape_texture, nullptr, &pos_dst);
		SDL_DestroyTexture(escape_texture);

		SDL_Texture* readiness_texture = get_text_texture(window, window.get_font(FontSize::SMALL), "Press 'Enter' when you're ready!", text_color);
		SDL_QueryTexture(readiness_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
		pos_dst.x = window.get_width() - pos_dst.w - 50;
		SDL_RenderCopy(window.get_renderer(), readiness_texture, nullptr, &pos_dst);
		SDL_DestroyTexture(readiness_texture);

		window.render_present();

		bool are_all_players_ready = true;
		for (const Player& player : player_infos) {
			if (!player.get_is_ready()) {
				are_all_players_ready = false;
				break;
			}
		}
	}
}


void battle(Window& window, TCPsocket socket, SDLNet_SocketSet socket_set, std::vector<Player>& player_infos, const Pokemons& pokemons) {
	window.load_and_play_music("data/red.ogg");

	const std::string my_nickname = window.get_settings().get_nickname();
	std::vector<std::shared_ptr<Player>> other_players;
	std::shared_ptr<Player> my_player_ptr;

	for (auto& player : player_infos) {
		std::shared_ptr<Player> player_ptr = std::make_shared<Player>(player);
		if (player.get_nickname() != my_nickname)
			other_players.emplace_back(player_ptr);
		else
			my_player_ptr = player_ptr;
	}

	Menu menu("Who do you want to attack?", window.get_font(FontSize::NORMAL), 600);

	int num_menu_choice = 0;
	for (int i = 0; i < other_players.size(); i++) {
		if (!other_players[i]->get_is_dead()) {
			menu.add_choice(num_menu_choice, other_players[i]->get_nickname());
			num_menu_choice++;
		}
	}

	while (true) {
		bool chosen = false;
		std::string nickname_to_attack;
		std::string attack;

		while ((nickname_to_attack.empty() || attack.empty()) && !my_player_ptr->get_is_dead()) {
			do {
				window.update_events();

				if (window.is_quit())
					exit(1);

				if (window.is_enter_down()) {
					window.press_up_key(SDL_SCANCODE_RETURN);
					window.press_up_key(SDL_SCANCODE_KP_ENTER);

					nickname_to_attack = menu.get_selected_choice_str();
					chosen = true;
				}

				std::vector<std::vector<std::string>> splitted_message = recv_network_splitted_msg_from_server_non_blocking(window, socket_set, socket);
				for (auto& message : splitted_message) {
					if (message[0] == "TO_CLIENT_PLAYER_INFOS") {
						player_infos.clear();
						for (int i = 1; i < message.size(); i += 5) {
							player_infos.emplace_back(std::stoi(message[i + 4]), std::stoi(message[i + 3]), message[i], std::stoi(message[i + 1]), (bool)std::stoi(message[i + 2]));
						}

						other_players.clear();

						for (auto& player : player_infos) {
							std::shared_ptr<Player> player_ptr = std::make_shared<Player>(player);
							if (player.get_nickname() != my_nickname)
								other_players.emplace_back(player_ptr);
							else
								my_player_ptr = player_ptr;
						}

						menu.clear_choices();

						int num_menu_choice = 0;
						for (int i = 0; i < other_players.size(); i++) {
							if (!other_players[i]->get_is_dead()) {
								menu.add_choice(num_menu_choice, other_players[i]->get_nickname());
								num_menu_choice++;
							}
						}

						if (num_menu_choice == 0) {
							return;
						}

						nickname_to_attack.clear();
					} else if (message[0] == "TO_CLIENT_END_GAME") {
						Message::show_message_battle(window, message[1] + " won! Congratulations!", true, pokemons, my_player_ptr, other_players);
						return;
					}
				}



				menu.update_selected_choice_from_events(window.get_events());

				window.render_clear();
				render_battlefield(window, pokemons, my_player_ptr, other_players);
				menu.render_menu(window);
				window.render_present();
			} while (nickname_to_attack.empty());

			attack.clear();
			
			Menu attack_menu("Which attack?", window.get_font(FontSize::NORMAL), 600);
			for (int attack_num = 0; attack_num < PKMN_ATTACKS[my_player_ptr->get_chosen_pokemon()].size(); attack_num++) {
				attack_menu.add_choice(attack_num, PKMN_ATTACKS[my_player_ptr->get_chosen_pokemon()][attack_num]);
			}

			do {
				window.update_events();

				if (window.is_quit())
					exit(1);

				if (window.is_enter_down()) {
					window.press_up_key(SDL_SCANCODE_RETURN);
					window.press_up_key(SDL_SCANCODE_KP_ENTER);

					attack = attack_menu.get_selected_choice_str();
				}

				std::vector<std::vector<std::string>> splitted_message = recv_network_splitted_msg_from_server_non_blocking(window, socket_set, socket);
				for (auto& message : splitted_message) {
					if (message[0] == "TO_CLIENT_PLAYER_INFOS") {
						player_infos.clear();
						for (int i = 1; i < message.size(); i += 5) {
							player_infos.emplace_back(std::stoi(message[i + 4]), std::stoi(message[i + 3]), message[i], std::stoi(message[i + 1]), (bool)std::stoi(message[i + 2]));
						}

						other_players.clear();

						for (auto& player : player_infos) {
							std::shared_ptr<Player> player_ptr = std::make_shared<Player>(player);
							if (player.get_nickname() != my_nickname)
								other_players.emplace_back(player_ptr);
							else
								my_player_ptr = player_ptr;
						}

						menu.clear_choices();

						int num_menu_choice = 0;
						for (int i = 0; i < other_players.size(); i++) {
							if (!other_players[i]->get_is_dead()) {
								menu.add_choice(num_menu_choice, other_players[i]->get_nickname());
								num_menu_choice++;
							}
						}

						if (num_menu_choice == 0) {
							return;
						}

						nickname_to_attack.clear();
						attack = "exit from this loop...";
					} else if (message[0] == "TO_CLIENT_END_GAME") {
						Message::show_message_battle(window, message[1] + " won! Congratulations!", true, pokemons, my_player_ptr, other_players);
						return;
					}
				}

				attack_menu.update_selected_choice_from_events(window.get_events());

				window.render_clear();
				render_battlefield(window, pokemons, my_player_ptr, other_players);
				attack_menu.render_menu(window);
				window.render_present();
			} while (attack.empty());
		}


		if (!my_player_ptr->get_is_dead()) {
			std::string message = "TO_SERVER_ATTACK";
			message += MESSAGE_WORD_DELIMITER + attack + MESSAGE_WORD_DELIMITER + nickname_to_attack + MESSAGE_DELIMITER;
			SDLNet_TCP_Send(socket, message.c_str(), message.size());
		}
		
		int alpha = 255;
		bool alpha_increase = false;
		Uint64 last_ticks = SDL_GetTicks64();

		
		std::vector<std::vector<std::string>> splitted_message;

		do {
			window.update_events();

			if (window.is_quit())
				exit(0);

			splitted_message = recv_network_splitted_msg_from_server_non_blocking(window, socket_set, socket);

			window.render_clear();
			Uint64 ticks = SDL_GetTicks64();

			SDL_Color text_color = { 0, 0, 0, alpha };
			Uint64 diff = ticks - last_ticks;
			if (alpha_increase) {
				alpha += diff / 2;
				if (alpha >= 255) {
					alpha = 255;
					alpha_increase = false;
				}
			}
			else {
				alpha -= diff / 2;
				if (alpha <= 1) {
					alpha = 1;
					alpha_increase = true;
				}
			}

			last_ticks = ticks;

			render_battlefield(window, pokemons, my_player_ptr, other_players);

			SDL_Rect pos_dst;
			SDL_Texture* texture = get_text_texture(window, window.get_font(FontSize::NORMAL), "Waiting for other players...", text_color);
			SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
			pos_dst.x = window.get_width() / 2 - pos_dst.w / 2;
			pos_dst.y = window.get_height() - pos_dst.h - 80;
			SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
			SDL_DestroyTexture(texture);

			window.render_present();
		} while (splitted_message.empty());

		for (auto& message : splitted_message) {
			if (message[0] == "TO_CLIENT_ATTACK") {
				Message::show_message_battle(window, message[3] + " used " + message[1] + " on " + message[2] + "!", true, pokemons, my_player_ptr, other_players);

				std::shared_ptr<Player> hurted_player = nullptr;
				for (auto& player : other_players) {
					if (player->get_nickname() == message[2]) {
						hurted_player = player;
						break;
					}
				}

				if (hurted_player == nullptr)
					hurted_player = my_player_ptr;

				if (hurted_player->get_is_dead())
					Message::show_message_battle(window, "But " + message[2] + " is already KO...", true, pokemons, my_player_ptr, other_players);
				else {
					window.play_sound(Sound::HIT);

					Uint64 first_ticks = SDL_GetTicks64();
					Uint64 current_ticks = first_ticks;

					while (current_ticks - first_ticks < 1000) {
						window.update_events();

						if (window.is_quit())
							exit(1);

						window.render_clear();

						const bool show_pokemon = current_ticks % 500 < 250;
						hurted_player->set_show_pokemon(show_pokemon);

						render_battlefield(window, pokemons, my_player_ptr, other_players);

						window.render_present();

						current_ticks = SDL_GetTicks64();
					}

					hurted_player->set_show_pokemon(true);

					int damage_to_decrease_left = std::stoi(message[4]);

					while (damage_to_decrease_left > 0) {
						window.update_events();

						if (window.is_quit())
							exit(1);

						window.render_clear();

						hurted_player->decrease_health(1);
						int health_left = hurted_player->get_health();
						if (health_left > 0)
							damage_to_decrease_left--;
						else {
							const std::string ko_nickname = hurted_player->get_nickname();
							Message::show_message_battle(window, ko_nickname + " is KO", true, pokemons, my_player_ptr, other_players);
							damage_to_decrease_left = 0;
						}

						render_battlefield(window, pokemons, my_player_ptr, other_players);

						window.render_present();
					}

					const bool is_critical = (bool)std::stoi(message[5]);
					if (is_critical)
						Message::show_message_battle(window, "Critical hit!", true, pokemons, my_player_ptr, other_players);
				}
			} else if (message[0] == "TO_CLIENT_END_GAME") {
				Message::show_message_battle(window, message[1] + " won! Congratulations!", true, pokemons, my_player_ptr, other_players);
				return;
			}
		}


		for (auto& message : splitted_message) {
			if (message[0] == "TO_CLIENT_PLAYER_INFOS") {
				player_infos.clear();
				for (int i = 1; i < message.size(); i += 5) {
					player_infos.emplace_back(std::stoi(message[i + 4]), std::stoi(message[i + 3]), message[i], std::stoi(message[i + 1]), (bool)std::stoi(message[i + 2]));
				}

				other_players.clear();

				for (auto& player : player_infos) {
					std::shared_ptr<Player> player_ptr = std::make_shared<Player>(player);
					if (player.get_nickname() != my_nickname)
						other_players.emplace_back(player_ptr);
					else
						my_player_ptr = player_ptr;
				}
			} else if (message[0] == "TO_CLIENT_END_GAME") {
				Message::show_message_battle(window, message[1] + " won! Congratulations!", true, pokemons, my_player_ptr, other_players);
				return;
			}
		}

		menu.clear_choices();

		int num_menu_choice = 0;
		for (int i = 0; i < other_players.size(); i++) {
			if (!other_players[i]->get_is_dead()) {
				menu.add_choice(num_menu_choice, other_players[i]->get_nickname());
				num_menu_choice++;
			}
		}
	}
}


void render_battlefield(Window& window, const Pokemons& pokemons, const std::shared_ptr<Player>& my_player_ptr, const std::vector<std::shared_ptr<Player>>& other_players_ptr) {
	window.get_texture(Picture::FIGHT_BACKGROUND)->render_without_pos_dst(window);

	if (!my_player_ptr->get_is_dead()) {
		if (my_player_ptr->get_show_pokemon()) {
			std::shared_ptr<Anim> my_anim = pokemons.get_pokemon_back_anim_ptr(my_player_ptr->get_chosen_pokemon());
			my_anim->set_pos_dst(120, window.get_height() - 380);
			my_anim->render_anim(window, true);
		}

		Hud::show_pokemon_hud(window, my_player_ptr->get_nickname(), my_player_ptr->get_initial_health(), my_player_ptr->get_health(), 80, window.get_height() - 460);
	}
	
	

	for (int i = 0; i < other_players_ptr.size(); i++) {
		if (!other_players_ptr[i]->get_is_dead()) {
			std::shared_ptr<Anim> anim_ptr = pokemons.get_pokemon_anim_ptr(other_players_ptr[i]->get_chosen_pokemon());

			if (other_players_ptr[i]->get_show_pokemon()) {
				anim_ptr->set_pos_dst(window.get_width() - 160 - anim_ptr->get_width() - i * 250, 135 - anim_ptr->get_height() / 2);
				anim_ptr->render_anim(window, true);
			}

			Hud::show_pokemon_hud(window, other_players_ptr[i]->get_nickname(), other_players_ptr[i]->get_initial_health(), other_players_ptr[i]->get_health(), window.get_width() - 200 - anim_ptr->get_width() - i * 250, 215);
		}
	}
}