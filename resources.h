//
// Created by bfloch on 3/15/21.
//

#ifndef PETIT_PROGRAM_RESOURCES_H
#define PETIT_PROGRAM_RESOURCES_H

#include <SDL.h>
#include <array>
#include <cassert>
constexpr int PLAYER_WIDTH = 7;
constexpr int PLAYER_HEIGHT = 11;
constexpr int PLAYER_FRAMES = 2;
constexpr unsigned char PLAYER[] =
    "  $$$  "
    "  $$!  "
    "  $$$  "
    "  !!!! "
    "  !!!! "
    "  !!!  "
    "  ###  "
    " ##### "
    " ## ## "
    " ## ## "
    " #!!#!!"

    "   $$$ "
    "   $$! "
    "   $$$ "
    "   !!!!"
    "  !!!!!"
    "  !!!  "
    "  ###  "
    " ##### "
    " ## ## "
    " ## ## "
    " #!!#!!";


static SDL_Color mapColor(unsigned char colorCode)
{
    constexpr unsigned char firstEntryOffset = ' ';

    static const std::array<SDL_Color, 5> palette{{
        {.r = 0, .g = 0, .b = 0, .a = 0},        // Space = Transparent
        {.r = 255, .g = 0, .b = 0, .a = 255},    // ! = Red
        {.r = 0, .g = 255, .b = 0, .a = 255},    // " = Green
        {.r = 0, .g = 0, .b = 255, .a = 255},    // # = Blue
        {.r = 255, .g = 255, .b = 255, .a = 255},// $ = White
    }};

    assert(colorCode >= firstEntryOffset && colorCode - firstEntryOffset < palette.size());

    return palette[colorCode - firstEntryOffset];
}

#endif//PETIT_PROGRAM_RESOURCES_H
