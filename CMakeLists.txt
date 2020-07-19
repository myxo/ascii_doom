project(ascii_doom)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /DUNICODE /utf-8")


add_library(olcConsoleGameEngine STATIC)
target_sources(olcConsoleGameEngine
PUBLIC
    third_party/olc/olc.h
PRIVATE
    third_party/olc/lib.cpp
    third_party/olc/olcConsoleGameEngine.h
)
target_compile_options(olcConsoleGameEngine PRIVATE /W2 /WX)

add_executable(ascii_doom)
target_sources(ascii_doom
PRIVATE
    src/main_doom.c
    src/world_object.c 
    src/logging.c
    src/render.c
    src/bullet.c
    src/enemy.c
    src/sprite.c
    src/player.c
    src/structs_of_data.c
    src/config.c 
)

add_executable(sprite_editor)
target_sources(sprite_editor
PRIVATE
	src/sprite_editor.c
	src/sprite.h
	src/sprite.c
)

file(GLOB resource_files "res/*")
file(COPY ${resource_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(ascii_doom olcConsoleGameEngine)
target_include_directories(ascii_doom PRIVATE third_party)
target_compile_options(ascii_doom PRIVATE /W2 /WX)

target_link_libraries(sprite_editor olcConsoleGameEngine)
target_include_directories(sprite_editor PRIVATE third_party)
target_compile_options(ascii_doom PRIVATE /W2 /WX)

