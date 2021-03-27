//
// Created by bfloch on 3/15/21.
//

#ifndef PETIT_PROGRAM_RESOURCES_H
#define PETIT_PROGRAM_RESOURCES_H

#include <SDL.h>
#include <array>
#include <cassert>

constexpr int PLAYER_WIDTH = 9;
constexpr int PLAYER_HEIGHT = 9;
constexpr int PLAYER_FRAMES = 8;
constexpr unsigned char PLAYER_DATA[] =
    "   $$$   "
    "  $$$$$  "
    " $$#$$$$ "
    "$$$$$$$$$"
    "$$$$     "
    "$$$$$$$$$"
    " $$$$$$$ "
    "  $$$$$  "
    "   $$$   "

    "   $$$   "
    "  $$$$$  "
    " $$#$$   "
    "$$$$     "
    "$$$      "
    "$$$$     "
    " $$$$$   "
    "  $$$$$  "
    "   $$$   "

    "   $$$   "
    "  $$$$$  "
    " $$$$$$$ "
    "$$$$$$$$$"
    "$$$$$$$$$"
    "$$$$$$$$$"
    " $$$$$$$ "
    "  $$$$$  "
    "   $$$   "

    "         "
    "   $$$   "
    "  $$$$$  "
    " $$$$$$$ "
    " $$$$$$$ "
    " $$$$$$$ "
    "  $$$$$  "
    "   $$$   "
    "         "

    "         "
    "         "
    "   $$$   "
    "  $$$$$  "
    "  $$$$$  "
    "  $$$$$  "
    "   $$$   "
    "         "
    "         "

    "         "
    "         "
    "         "
    "   $$$   "
    "   $$$   "
    "   $$$   "
    "         "
    "         "
    "         "

    "         "
    "         "
    "         "
    "         "
    "    $    "
    "         "
    "         "
    "         "
    "         "

    "         "
    "         "
    "         "
    "         "
    "         "
    "         "
    "         "
    "         "
    "         "
;

constexpr int POINT_WIDTH = 4;
constexpr int POINT_HEIGHT = 4;
constexpr int POINT_FRAMES = 2;
constexpr unsigned char POINT_DATA[] =
    "    "
    " !! "
    " !! "
    "    "

    // Note that \" acts as " (single character)
    " !! "
    "!\"\"!"
    "!\"\"!"
    " !! "
;

constexpr int NUMBERS_WIDTH = 5;
constexpr int NUMBERS_HEIGHT = 5;
constexpr int NUMBERS_FRAMES = 10;
constexpr unsigned char NUMBERS_DATA[] =
    " !!! "
    "!   !"
    "!   !"
    "!   !"
    " !!! "

    "  !! "
    " ! ! "
    "   ! "
    "   ! "
    "   ! "

    "  !! "
    " !  !"
    "   ! "
    " !!  "
    " !!!!"

    " !!! "
    "    !"
    "  !! "
    "    !"
    " !!! "

    "   !!"
    "  ! !"
    " !  !"
    " !!!!"
    "    !"

    " !!!!"
    " !   "
    " !!! "
    "    !"
    " !!! "

    "  !!!"
    " !   "
    " !!! "
    " !  !"
    " !!! "

    " !!!!"
    "    !"
    "   ! "
    "  !  "
    "  !  "

    " !!! "
    "!   !"
    " !!! "
    "!   !"
    " !!! "

    " !!! "
    "!   !"
    " !!!!"
    "    !"
    " !!! "
;

static const std::array<SDL_Color, 5> palette1{{
    {.r = 0, .g = 0, .b = 0, .a = 0},        // Space
    {.r = 255, .g = 255, .b = 255, .a = 255},    // !
    {.r = 200, .g = 128, .b = 64, .a = 255},    // "
    {.r = 255, .g = 140, .b = 180, .a = 255},// #
    {.r = 255, .g = 255, .b = 0, .a = 255},      // $
}};

static const std::array<SDL_Color, 5> palette2{{
    {.r = 0, .g = 0, .b = 0, .a = 0},        // Space
    {.r = 255, .g = 0, .b = 0, .a = 255},    // !
    {.r = 200, .g = 128, .b = 64, .a = 255},    // "
    {.r = 255, .g = 140, .b = 180, .a = 255},// #
    {.r = 0, .g = 128, .b = 0, .a = 255},  // $
}};

#endif//PETIT_PROGRAM_RESOURCES_H
