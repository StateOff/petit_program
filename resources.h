//
// Created by bfloch on 3/15/21.
//

#ifndef PETIT_PROGRAM_RESOURCES_H
#define PETIT_PROGRAM_RESOURCES_H

#include <SDL.h>
#include <array>

constexpr int PLAYER_WIDTH = 9;
constexpr int PLAYER_HEIGHT = 9;
constexpr int PLAYER_FRAMES = 9;
constexpr unsigned char PLAYER_DATA[] =
    "   $$$   "
    "  $#$$$  "
    " $$#$$$$ "
    "$$$$$$$$$"
    "$$$$$    "
    "$$$$     "
    "$$$$$$$$ "
    " $$$$$$$ "
    "  $$$$   "

    "   $$$   "
    "  $#$$$$ "
    " $$#$$   "
    "$$$$     "
    "$$$      "
    "$$$      "
    "$$$$     "
    " $$$$$$  "
    "  $$$$   "

    "   $$$   "
    "  $$$$$  "
    " $#$$$#$ "
    "$$#$$$#$$"
    "$$$$$$$$$"
    "$$$   $$$"
    " $ $$$ $ "
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
    "         ";

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
    " !! ";

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
    " !!! ";

constexpr int TEXT_WIDTH = 5;
constexpr int TEXT_HEIGHT = 5;
constexpr int TEXT_FRAMES = 12;
constexpr unsigned char TEXT_DATA[] =
    // 0
    "     "
    "     "
    "     "
    "     "
    "     "

    // 1
    "!    "
    "!    "
    "!    "
    "!    "
    "!!!! "

    // 2
    "!!!! "
    "!    "
    "!!!  "
    "!    "
    "!!!! "

    // 3
    "!   !"
    "!   !"
    "!   !"
    " ! ! "
    "  !  "

    // 4
    " !!!!"
    "!    "
    "!  !!"
    "!   !"
    " !!! "

    // 5
    "  !  "
    " ! ! "
    "!   !"
    "!!!!!"
    "!   !"

    // 6
    "!   !"
    "!! !!"
    "! ! !"
    "!   !"
    "!   !"

    // 7
    " !!! "
    "!   !"
    "!   !"
    "!   !"
    " !!! "

    // 8
    "!!!! "
    "!   !"
    "!!!! "
    "!  ! "
    "!   !"

    // 9
    "!!!! "
    "!   !"
    "!   !"
    "!   !"
    "!!!! "

    // 10
    "!   !"
    " ! ! "
    "  !  "
    "  !  "
    "  !  "

    // 11
    " !!  "
    "!  ! "
    "  !  "
    "     "
    "  !  ";


static const std::array<SDL_Color, 5> palette1{{
    {.r = 0, .g = 0, .b = 0, .a = 0},        // Space
    {.r = 255, .g = 255, .b = 255, .a = 255},// !
    {.r = 200, .g = 128, .b = 64, .a = 255}, // "
    {.r = 226, .g = 240, .b = 220, .a = 255},// #
    {.r = 0, .g = 128, .b = 0, .a = 255},    // $
}};

static const std::array<SDL_Color, 5> palette2{{
    {.r = 0, .g = 0, .b = 0, .a = 0},        // Space
    {.r = 255, .g = 0, .b = 0, .a = 255},    // !
    {.r = 200, .g = 128, .b = 64, .a = 255}, // "
    {.r = 226, .g = 240, .b = 220, .a = 255},// #
    {.r = 124, .g = 180, .b = 255, .a = 255},// $
}};

#endif//PETIT_PROGRAM_RESOURCES_H
