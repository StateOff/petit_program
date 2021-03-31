//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_ENTITY_TEXT_H
#define PETIT_PROGRAM_ENTITY_TEXT_H

#include <cmath>

class Text: public SpriteEntity
{

public:
    Text(const SpriteAnimationType& spriteAnimations, bool isRightAligned)
    : m_isRightAligned(isRightAligned)
    {
        auto s = std::make_unique<Sprite>();
        s->setAnimation(spriteAnimations[SPRITE_ANIMATION_TEXT_LEVEL].get());
        s->stop();
        size_t spriteIndex = addComponent(std::move(s));
        assert(spriteIndex == SPRITE_COMPONENT_INDEX);
    }

    void update(Uint32 deltaTime) override
    {
        Entity::update(deltaTime);
    }

    void render(SDL_Renderer* renderer) override
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        int characters = sprite->frames();

        int oldX = sprite->x;

        if(m_isRightAligned)
            sprite->x -= (characters - 1) * (width() + 1);

        for(int i = 0; i < characters; ++i)
        {
            sprite->setFrame(i);
            Entity::render(renderer);
            sprite->x += width() + 1;
        }

        sprite->x = oldX;
    }

    void setText(SpriteAnimation* animation)
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        sprite->setAnimation(animation);
        sprite->stop();
    }

    int height() const override
    {
        return TEXT_WIDTH;
    }
    int width() const override
    {
        return TEXT_HEIGHT;
    }

private:
    bool m_isRightAligned{false};
};

#endif//PETIT_PROGRAM_ENTITY_TEXT_H
