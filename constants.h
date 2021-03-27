//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_CONSTANTS_H
#define PETIT_PROGRAM_CONSTANTS_H

constexpr const char* APP_NAME = "Petit Program";

// The resolution of the game
constexpr int BACKBUFFER_WIDTH = 256;
constexpr int BACKBUFFER_HEIGHT = 144;

// The Window / Screen
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

// Shows the collision rectangles
constexpr bool DEBUG_COLLISION_RECT = false;
constexpr int FAST_BLINK_TIME = 10 * 1000;
constexpr double FAST_BLINK_SPEED = 0.4;

enum SpriteNames
{
    SPRITE_PLAYER_1,
    SPRITE_PLAYER_2,
    SPRITE_POINT,
    SPRITE_NUMBERS,
    SPRITE_NUM
};

enum SpriteAnimationNames
{
    SPRITE_ANIMATION_PLAYER_1_IDLE,
    SPRITE_ANIMATION_PLAYER_1_EAT,
    SPRITE_ANIMATION_PLAYER_1_BITTEN,
    SPRITE_ANIMATION_PLAYER_2_IDLE,
    SPRITE_ANIMATION_PLAYER_2_EAT,
    SPRITE_ANIMATION_PLAYER_2_BITTEN,
    SPRITE_ANIMATION_POINT_SMALL,
    SPRITE_ANIMATION_POINT_BIG,
    SPRITE_ANIMATION_POINT_BLINK,
    SPRITE_ANIMATION_NUMBERS,
    SPRITE_ANIMATION_NUM
};

using SpriteAnimationType = std::array<std::unique_ptr<SpriteAnimation>, SPRITE_ANIMATION_NUM>;

#endif//PETIT_PROGRAM_CONSTANTS_H
