#pragma once

#include <vector>
#include <string>
#include <SDL2/SDL_net.h>

std::vector<std::vector<std::string>> recv_network_splitted_msg_non_blocking(SDLNet_SocketSet socket_set, TCPsocket socket);