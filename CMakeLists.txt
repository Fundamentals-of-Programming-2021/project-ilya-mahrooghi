cmake_minimum_required(VERSION 3.16)
project("state.io")
include_directories("/usr/include/SDL2")

include_directories("include/game")
include_directories("include/menu")
include_directories("include/AI")
include_directories("include/endofgame")
include_directories("include")

file(GLOB_RECURSE SOURCE "src/*.c" )
add_executable(main "${SOURCE}")

target_link_libraries(main m SDL2 SDL2_gfx SDL2_ttf)