# CMakeList.txt : CMake project for pokefight-server, include source and define
# project specific logic here.
#

# Add source to this project's executable.
set (SRC "main.cpp" "../pokefight-common/ip.cpp" "../pokefight-common/ip.h" "../pokefight-common/player.h" "../pokefight-common/player.cpp" "attack.cpp" "attack.h" "colors.h" "colors.cpp" "../pokefight-common/const.h") 
add_executable ("pokefight-server" ${SRC} "res.rc")

include_directories (SRC ${SDL2_INCLUDE_DIR} ${SDL2_NET_INCLUDE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET "pokefight-server" PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(
        TARGET "pokefight-server" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/dependencies/win64-msvc/bin/SDL2.dll
                ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
        TARGET "pokefight-server" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/dependencies/win64-msvc/bin/SDL2_net.dll
                ${CMAKE_CURRENT_BINARY_DIR})

# TODO: Add tests and install targets if needed.
