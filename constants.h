//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_CONSTANTS_H
#define PETIT_PROGRAM_CONSTANTS_H

#include <array>
#include <memory>

#include "sprite_animation.h"

constexpr const char* APP_NAME = "Petit Program";

// The resolution of the game
constexpr int BACKBUFFER_WIDTH = 256;
constexpr int BACKBUFFER_HEIGHT = 144;

// The Window / Screen
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

#ifndef NDEBUG
// Shows the collision rectangles
constexpr bool DEBUG_COLLISION_RECT = false;
#endif

enum SpriteNames
{
    SPRITE_PLAYER_1,
    SPRITE_PLAYER_2,
    SPRITE_POINT,
    SPRITE_NUMBERS,
    SPRITE_TEXT,
    SPRITE_NUM
};

enum SpriteAnimationName
{
    SPRITE_ANIMATION_PLAYER_1_IDLE,
    SPRITE_ANIMATION_PLAYER_1_EAT,
    SPRITE_ANIMATION_PLAYER_1_FEAR,
    SPRITE_ANIMATION_PLAYER_1_BITTEN,
    SPRITE_ANIMATION_PLAYER_2_IDLE,
    SPRITE_ANIMATION_PLAYER_2_EAT,
    SPRITE_ANIMATION_PLAYER_2_FEAR,
    SPRITE_ANIMATION_PLAYER_2_BITTEN,
    SPRITE_ANIMATION_POINT_SMALL,
    SPRITE_ANIMATION_POINT_BIG,
    SPRITE_ANIMATION_POINT_BLINK,
    SPRITE_ANIMATION_NUMBERS,
    SPRITE_ANIMATION_TEXT_LEVEL,
    SPRITE_ANIMATION_TEXT_GAME_OVER,
    SPRITE_ANIMATION_TEXT_READY,
    SPRITE_ANIMATION_NUM
};

using SpriteAnimationType = std::array<std::unique_ptr<SpriteAnimation>, SPRITE_ANIMATION_NUM>;

#endif//PETIT_PROGRAM_CONSTANTS_H
