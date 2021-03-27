//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_ENTITY_POINT_H
#define PETIT_PROGRAM_ENTITY_POINT_H

class Point: public SpriteEntity
{

public:
    Point(const SpriteAnimationType& spriteAnimations)
    {
        auto s = std::make_unique<Sprite>();
        s->setAnimation(spriteAnimations[SPRITE_ANIMATION_POINT_BLINK].get());
        s->setSpeed(0.1);
        size_t spriteIndex = addComponent(std::move(s));
        assert(spriteIndex == SPRITE_COMPONENT_INDEX);
    }

    void update(Uint32 deltaTime) override
    {
        Entity::update(deltaTime);
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
    }

    int height() const override
    {
        return POINT_HEIGHT;
    }
    int width() const override
    {
        return POINT_WIDTH;
    }

    void setSpeed(double speed)
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        sprite->setSpeed(speed);
    }
};

#endif//PETIT_PROGRAM_ENTITY_POINT_H
