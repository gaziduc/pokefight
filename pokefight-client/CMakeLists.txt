# CMakeList.txt : CMake project for pokefight-client, include source and define
# project specific logic here.
#

# Add source to this project's executable.
set (SRC "main.cpp" "main.h" "window.cpp" "window.h" "anim.cpp" "anim.h" "events.cpp" "events.h" "menu.h" "menu.cpp" "texture.h" "texture.cpp" "pictures.cpp" "pictures.h" "input.cpp" "input.h" "utils.cpp" "utils.h" "message.cpp" "message.h" "pokemons.cpp" "pokemons.h" "../pokefight-common/ip.cpp" "../pokefight-common/ip.h" "../pokefight-common/player.h" "../pokefight-common/player.cpp" "settings.cpp" "settings.h" "../pokefight-common/const.h")
add_executable ("pokefight-client" ${SRC} "res.rc")

include_directories (SRC ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIR} ${SDL2_NET_INCLUDE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET "pokefight-client" PROPERTY CXX_STANDARD 20)
endif()


add_custom_command(
        TARGET "pokefight-client" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/data
                ${CMAKE_CURRENT_BINARY_DIR}/data)

add_custom_command(
        TARGET "pokefight-client" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/dependencies/win64-msvc/bin
                ${CMAKE_CURRENT_BINARY_DIR})

# TODO: Add tests and install targets if needed.
