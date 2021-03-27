//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_ENTITY_COUNTER_H
#define PETIT_PROGRAM_ENTITY_COUNTER_H

#include <cmath>
class Counter: public SpriteEntity
{

public:
    Counter(const SpriteAnimationType& spriteAnimations, bool isRightAligned)
    : m_isRightAligned(isRightAligned)
    {
        auto s = std::make_unique<Sprite>();
        s->setAnimation(spriteAnimations[SPRITE_ANIMATION_NUMBERS].get());
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
        sprite->setFrame(m_number);

        int oldX = sprite->x;

        int decimals = m_number == 0 ? 1 : 1 + std::floor(log10(m_number));
        if(m_isRightAligned)
            sprite->x -= (decimals - 1) * (width() + 1);

            int cur = m_number;
        for(int i = decimals; i > 0; --i)
        {
            int base = pow(10, i-1);
            int left = cur % base;
            int number = (cur - left) / base;
            cur = left;

            sprite->setFrame(number);
            Entity::render(renderer);
            sprite->x += width() + 1;
        }

        sprite->x = oldX;
    }

    void setNumber(int number)
    {
        m_number = number;
    }

    int height() const override
    {
        return NUMBERS_WIDTH;
    }
    int width() const override
    {
        return NUMBERS_HEIGHT;
    }

private:
    int m_number{0};
    bool m_isRightAligned{false};
};

#endif//PETIT_PROGRAM_ENTITY_COUNTER_H
