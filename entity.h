//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_ENTITY_H
#define PETIT_PROGRAM_ENTITY_H

#include "component.h"
#include "component_sprite.h"
#include "constants.h"
#include "debug.h"
#include "game.h"
#include "service_locator.h"
#include <SDL.h>
class Entity
{
public:
    virtual void update(Uint32 deltaTime)
    {
        for(auto& component : m_components)
        {
            component->update(deltaTime);
        }
    }

    virtual void render(SDL_Renderer* renderer)
    {
        for(auto& component : m_components)
        {
            component->render(renderer);
        }
    }

    size_t addComponent(std::unique_ptr<Component> component)
    {
        m_components.emplace_back(std::move(component));
        return m_components.size() - 1;
    }

    Component* getComponent(size_t index)
    {
        return m_components[index].get();
    }

    [[nodiscard]] Component* getComponent(size_t index) const
    {
        return m_components[index].get();
    }
private:
    std::vector<std::unique_ptr<Component>> m_components;
};

class SpriteEntity : public Entity
{
public:
    constexpr static size_t SPRITE_COMPONENT_INDEX = 0;

    void setPosition(int x, int y)
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        sprite->x = x;
        sprite->y = y;
    }

    void setAnimation(SpriteAnimation* animation)
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        sprite->setAnimation(animation);
    }

    void setAnimationSpeed(double speed)
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        sprite->setSpeed(speed);
    }

    void setAnimationFrame(int frame)
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        sprite->setFrame(frame);
    }

    SDL_Rect collisionRect()
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        SDL_Rect rect;
        rect.x = sprite->x;
        rect.y = sprite->y;
        rect.w = width();
        rect.h = height();
        return rect;
    }

    virtual int height() const = 0;
    virtual int width() const = 0;

    [[nodiscard]] int x() const {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        return sprite->x;
    }

    [[nodiscard]] int y() const {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        return sprite->y;
    }
    void freeze()
    {
        m_isFrozen = true;
    }
protected:
    bool m_isFrozen {false};
};

#endif//PETIT_PROGRAM_ENTITY_H
