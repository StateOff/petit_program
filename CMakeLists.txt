cmake_minimum_required(VERSION 3.17)
project(petit_program)

set(CMAKE_CXX_STANDARD 17)

set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory( contrib/SDL )

add_executable(petit_program main.cpp component_sprite.h resources.h service_locator.h graphics.h sprite_resource.h sprite_animation.h game.h service_input.h entity.h entity_player.h constants.h debug.h entity_point.h entity_counter.h entity_enemy.h levels.h)

target_link_libraries(petit_program SDL2main SDL2-static )
target_include_directories(petit_program PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/contrib/SDL/include)

add_executable(test_petit_program main.cpp component_sprite.h resources.h service_locator.h)

target_link_libraries(test_petit_program SDL2main SDL2-static )
target_include_directories(test_petit_program PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/contrib/SDL/include)
target_compile_definitions(test_petit_program PRIVATE PETIT_TESTS)
