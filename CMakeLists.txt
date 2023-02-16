# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.13)

project ("pokefight")

# Include sub-projects.
add_subdirectory ("pokefight-client")
add_subdirectory ("pokefight-server")

# dependencies headers
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set (DEPS_DIR ${PROJECT_SOURCE_DIR}/dependencies/win64-msvc)

find_package (SDL2 REQUIRED) 
find_package (SDL2_IMAGE REQUIRED)
find_package (SDL2_TTF REQUIRED)
find_package (SDL2_MIXER REQUIRED)
find_package (SDL2_GFX REQUIRED)
find_package (SDL2_NET REQUIRED)

# dependencies libraries
target_link_libraries ("pokefight-client" ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_GFX_LIBRARY} ${SDL2_NET_LIBRARY})
set_target_properties ("pokefight-client" PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)

# dependencies libraries
target_link_libraries ("pokefight-server" ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY} ${SDL2_NET_LIBRARY})