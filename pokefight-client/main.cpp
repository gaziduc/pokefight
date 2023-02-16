// main.cpp : Defines the entry point for the application.
//

#include "window.h"
#include "anim.h"
#include "events.h"
#include "menu.h"
#include "texture.h"
#include "input.h"
#include "utils.h"
#include "message.h"
#include "pokemons.h"
#include "../pokefight-common/ip.h"
#include "../pokefight-common/player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include <optional>


void connect(Window& window, const Pokemons& pokemons);
bool wait_for_players_readiness(Window& window, TCPsocket socket, SDLNet_SocketSet socket_set, std::vector<Player>& player_infos, const Pokemons& pokemons, const std::string& server_ip_port);
void battle(Window& window, TCPsocket socket, SDLNet_SocketSet socket_set, std::vector<Player>& player_infos, const Pokemons& pokemons);

int main(int argc, char *argv[])
{
	Window window(1280, 720);

	try {
		window.init();

		Pokemons pokemon_list;
		pokemon_list.add_pokemon("Venusaur", std::make_shared<Anim>(window, "data/anims/front/ani_bw_003_m.gif", 0, 0));
		pokemon_list.add_pokemon("Charizard", std::make_shared<Anim>(window, "data/anims/front/ani_bw_006.gif", 0, 0));
		pokemon_list.add_pokemon("Blastoise", std::make_shared<Anim>(window, "data/anims/front/ani_bw_009.gif", 0, 0));

		Menu menu(std::nullopt, window.get_font(FontSize::NORMAL));
		menu.add_choice(MainMenu::FIGHT_ONLINE_PLAYERS, "Fight online players");
		menu.add_choice(MainMenu::FIGHT_AGAINST_COMPUTER, "Fight against computer");
		menu.add_choice(MainMenu::SETTINGS, "Settings");
		menu.add_choice(MainMenu::QUIT, "Quit");

		Texture title_texture(window, "data/title.png");

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

				switch (menu.get_selected_choice()) {
				case MainMenu::FIGHT_ONLINE_PLAYERS:
					connect(window, pokemon_list);
					break;
				case MainMenu::FIGHT_AGAINST_COMPUTER:
					break;
				case MainMenu::SETTINGS:
					window.settings_menu(pokemon_list, title_texture);
					break;
				case MainMenu::QUIT:
					exit(0);

				default:
					break;
				}
			}

			menu.update_selected_choice_from_events(window.get_events());

			window.render_clear();
			title_texture.set_pos_dst(window.get_width() / 2 - title_texture.get_width() / 2, 80);
			title_texture.render(window);
			std::shared_ptr<Anim> chosed_anim_ptr = pokemon_list.get_pokemon_anim_ptr(window.get_settings().get_pokemon_num());
			chosed_anim_ptr->set_pos_dst(window.get_width() / 2 - chosed_anim_ptr->get_width() / 2, window.get_height() / 2 - chosed_anim_ptr->get_height() / 2 - 50);
			chosed_anim_ptr->render_anim(window);
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

void connect(Window& window, const Pokemons& pokemons) {
	Input input_ip("Enter server hostname or IP address:", window.get_font(FontSize::NORMAL), "");
	if (input_ip.show_input_menu(window) == InputEndAction::ESCAPE) {
		return;
	}

	Input input_port("Enter server port:", window.get_font(FontSize::NORMAL), "");
	if (input_port.show_input_menu(window) == InputEndAction::ESCAPE) {
		return;
	}

	IPaddress ip;
	int port = 0;
	try {
		port = std::stoi(input_port.get_current_input());
	}
	catch (const std::exception& exception) {
		return;
	}
	
	if (SDLNet_ResolveHost(&ip, input_ip.get_current_input().c_str(), port) == -1) {
		return;
	}

	TCPsocket socket = SDLNet_TCP_Open(&ip);
	if (socket == nullptr) {
		return;
	}

	// Send nickname and chosen pokemon to server
	std::string nickname_message("TO_SERVER_PLAYER_INFOS ");
	nickname_message += window.get_settings().get_nickname() + ' ' + std::to_string(window.get_settings().get_pokemon_num());
	SDLNet_TCP_Send(socket, nickname_message.c_str(), nickname_message.size());

	SDLNet_SocketSet socket_set = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socket_set, socket);
	
	std::vector<Player> nicknames;
	if (wait_for_players_readiness(window, socket, socket_set, nicknames, pokemons, input_ip.get_current_input() + ":" + std::to_string(port))) {
		battle(window, socket, socket_set, nicknames, pokemons);
	}


	SDLNet_TCP_Close(socket);
}

bool wait_for_players_readiness(Window& window, TCPsocket socket, SDLNet_SocketSet socket_set, std::vector<Player>& player_infos, const Pokemons& pokemons, const std::string& server_ip_port) {
	SDL_Color text_color = { 0, 0, 0, 255 };
	SDL_Color ready_text_color = { 117, 250, 97, 255 };
	SDL_Color not_ready_text_color = { 235, 51, 36, 255 };
	bool is_ready = false;

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
			std::string readiness_message("TO_SERVER_READINESS ");
			readiness_message += window.get_settings().get_nickname() + ' ' + std::to_string(is_ready);
			SDLNet_TCP_Send(socket, readiness_message.c_str(), readiness_message.size());
		}

		std::vector<std::vector<std::string>> splitted_message = recv_network_splitted_msg_from_server_non_blocking(socket_set, socket);
		if (!splitted_message.empty()) {
			player_infos.clear();
			std::vector<std::string> message = splitted_message[0];
			for (int i = 1; i < message.size(); i += 3) {
				player_infos.emplace_back(message[i], std::stoi(message[i + 1]), (bool) std::stoi(message[i + 2]));
			}
		}

		window.render_clear();

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

		pos_dst.x += 60;
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
			anim_ptr->render_anim(window);

			pos_dst.y += 65;
		}

		pos_dst.x = 50;
		pos_dst.y = window.get_height() - 50;

		SDL_Texture* escape_texture = get_text_texture(window, window.get_font(FontSize::SMALL), "Press 'Escape' to leave", text_color);
		SDL_QueryTexture(escape_texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
		pos_dst.y -= pos_dst.h;
		SDL_RenderCopy(window.get_renderer(), escape_texture, nullptr, &pos_dst);
		SDL_DestroyTexture(escape_texture);

		window.render_present();

		bool are_all_players_ready = true;
		for (const Player& player : player_infos) {
			if (!player.get_is_ready()) {
				are_all_players_ready = false;
				break;
			}
		}

		if (player_infos.size() >= 2 && are_all_players_ready) {
			return true;
		}
	}
}


void battle(Window& window, TCPsocket socket, SDLNet_SocketSet socket_set, std::vector<Player>& player_infos, const Pokemons& pokemons) {

	const std::string my_nickname = window.get_settings().get_nickname();
	std::vector<Player> other_players;
	Player my_player;

	for (auto& player : player_infos) {
		if (player.get_nickname() != my_nickname)
			other_players.emplace_back(player);
		else
			my_player = player;
	}

	while (true)
	{
		Menu menu("Who do you want to attack?", window.get_font(FontSize::NORMAL));
		for (int i = 0; i < other_players.size(); i++) {
			menu.add_choice(i, other_players[i].get_nickname());
		}

		bool chosen = false;
		while (!chosen) {
			window.update_events();

			if (window.is_quit())
				exit(1);

			if (window.is_enter_down()) {
				window.press_up_key(SDL_SCANCODE_RETURN);
				window.press_up_key(SDL_SCANCODE_KP_ENTER);

				const std::string nickname_to_attack = other_players[menu.get_selected_choice()].get_nickname();
				const std::string message = "TO_SERVER_ATTACK " + std::string("Tackle") + " " + nickname_to_attack;
				SDLNet_TCP_Send(socket, message.c_str(), message.size());

				chosen = true;
			}

			menu.update_selected_choice_from_events(window.get_events());

			window.render_clear();
			menu.render_menu(window);

			std::shared_ptr<Anim> my_anim = pokemons.get_pokemon_anim_ptr(my_player.get_chosen_pokemon());
			my_anim->set_pos_dst(80, window.get_height() - 300);
			my_anim->render_anim(window);

			for (int i = 0; i < other_players.size(); i++) {
				std::shared_ptr<Anim> anim = pokemons.get_pokemon_anim_ptr(other_players[i].get_chosen_pokemon());
				anim->set_pos_dst(window.get_width() - 100 - anim->get_width() - i * 150, 80);
				anim->render_anim(window);
			}
			
			window.render_present();
		}

		
		int alpha = 255;
		bool alpha_increase = false;
		Uint64 last_ticks = SDL_GetTicks64();

		
		std::vector<std::vector<std::string>> splitted_message;

		do {
			window.update_events();

			if (window.is_quit())
				exit(0);

			splitted_message = recv_network_splitted_msg_from_server_non_blocking(socket_set, socket);

			window.render_clear();
			Uint64 ticks = SDL_GetTicks();

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

			SDL_Rect pos_dst;
			SDL_Texture* texture = get_text_texture(window, window.get_font(FontSize::NORMAL), "Waiting for other players...", text_color);
			SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
			pos_dst.x = window.get_width() / 2 - pos_dst.w / 2;
			pos_dst.y = window.get_height() - 65;
			SDL_RenderCopy(window.get_renderer(), texture, nullptr, &pos_dst);
			SDL_DestroyTexture(texture);

			window.render_present();
		} while (splitted_message.empty());

		for (auto& message : splitted_message) {
			if (message[0] == "TO_CLIENT_ATTACK") {
				Message::show_message(window, message[3] + " used " + message[1] + " on " + message[2] + "!", true);
			}
		}
	}
}