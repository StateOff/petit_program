cmake_minimum_required(VERSION 3.17)
project(petit_program)

set(CMAKE_CXX_STANDARD 17)

set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory( contrib/SDL )

add_executable(petit_program main.cpp sprite.h resources.h)

target_link_libraries(petit_program SDL2main SDL2-static )
target_include_directories(petit_program PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/contrib/SDL/include)