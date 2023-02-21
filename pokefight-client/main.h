// pokefight.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "window.h"
#include "pokemons.h"
#include "../pokefight-common/player.h"
#include <string>
#include <SDL2/SDL_net.h>

std::string connect(Window& window, const Pokemons& pokemons);
bool wait_for_players_readiness(Window& window, TCPsocket socket, SDLNet_SocketSet socket_set, std::vector<Player>& player_infos, const Pokemons& pokemons, const std::string& server_ip_port);
void battle(Window& window, TCPsocket socket, SDLNet_SocketSet socket_set, std::vector<Player>& player_infos, const Pokemons& pokemons);
void render_battlefield(Window& window, const Pokemons& pokemons, const std::shared_ptr<Player>& my_player_ptr, const std::vector<std::shared_ptr<Player>>& other_players_ptr);

// TODO: Reference additional headers your program requires here.
