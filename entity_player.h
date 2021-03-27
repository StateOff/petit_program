//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_ENTITY_PLAYER_H
#define PETIT_PROGRAM_ENTITY_PLAYER_H

#include "component_sprite.h"
#include "constants.h"
#include "debug.h"
#include "game.h"
#include "service_locator.h"

class Player : public SpriteEntity
{

public:
    Player(const SpriteAnimationType& spriteAnimations, bool isFlipped)
    {
        auto s = std::make_unique<Sprite>();
        s->isFlipped = isFlipped;
        s->setAnimation(spriteAnimations[SPRITE_ANIMATION_PLAYER_1_EAT].get());
        s->setSpeed(5.0);
        size_t spriteIndex = addComponent(std::move(s));
        assert(spriteIndex == SPRITE_COMPONENT_INDEX);
    }

    void update(Uint32 deltaTime) override
    {
        Entity::update(deltaTime);
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        if(m_isFrozen) return;

        int dx = 0;
        int dy = 0;

        getInput(dx, dy);

        sprite->x += dx;
        sprite->y += dy;

        sprite->x = std::min(std::max(0, sprite->x), BACKBUFFER_WIDTH - PLAYER_WIDTH);
        sprite->y = std::min(std::max(NUMBERS_HEIGHT + 4, sprite->y), BACKBUFFER_HEIGHT - PLAYER_HEIGHT);

        if(dx < 0) {
            sprite->isFlipped = true;
        }
        if(dx > 0) {
            sprite->isFlipped = false;
        }
    }

    virtual void getInput(int& dx, int& dy) const
    {
        if(INPUT.isKeyPressed(SDL_SCANCODE_LEFT))
            dx -= m_speed;

        if(INPUT.isKeyPressed(SDL_SCANCODE_RIGHT))
            dx += m_speed;

        if(INPUT.isKeyPressed(SDL_SCANCODE_UP))
            dy -= m_speed;

        if(INPUT.isKeyPressed(SDL_SCANCODE_DOWN))
            dy += m_speed;
    }

    int height() const override
    {
        return PLAYER_HEIGHT;
    }
    int width() const override
    {
        return PLAYER_WIDTH;
    }

protected:
    int m_speed = 1;
};

#endif//PETIT_PROGRAM_ENTITY_PLAYER_H
