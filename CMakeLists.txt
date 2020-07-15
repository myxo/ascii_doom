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

add_executable(ascii_doom)
target_sources(ascii_doom 
PRIVATE
	src/main_doom.c
    src/world_object.h 
    src/world_object.c
    src/world_object.c 
	src/logging.h
	src/logging.c
	src/render.h
	src/render.c
  src/config.h
	src/config.c 
)
configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/res/map.txt
				${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/res/cfg.txt
				${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
target_link_libraries(ascii_doom olcConsoleGameEngine)
target_include_directories(ascii_doom PRIVATE third_party)

