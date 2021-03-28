//
// Created by bfloch on 3/15/21.
//

#ifndef PETIT_PROGRAM_COMPONENT_SPRITE_H
#define PETIT_PROGRAM_COMPONENT_SPRITE_H

#include "component.h"
#include "graphics.h"
#include "resources.h"
#include "sprite_animation.h"
#include "sprite_resource.h"
#include <SDL.h>
#include <cassert>
#include <memory>
#include <vector>


constexpr int CLIP_UNDEFINED = -1;


class Sprite: public Component
{
public:

    void render(SDL_Renderer* renderer) override
    {
        Component::render(renderer);
        assert(m_animation != nullptr);
        SDL_RendererFlip flip = isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        m_animation->render(renderer, x, y, flip);
    }
    void update(Uint32 deltaTime) override
    {
        Component::update(deltaTime);
        m_animation->update(deltaTime);
    }

    int frames() const
    {
        assert(m_animation != nullptr);
        return m_animation->frames();
    }

    void setSpeed(double speed)
    {
        m_animation->setSpeed(speed);
    }

    void stop()
    {
        m_animation->stop();
    }

    void play()
    {
        m_animation->play();
    }

    void setFrame(int frame)
    {
        m_animation->setFrame(frame);
    }

    void setAnimation(SpriteAnimation* animation)
    {
        m_animation = animation;
    }

    SpriteAnimation* m_animation {nullptr};
    int x{0};
    int y{0};
    bool isFlipped = false;
    int clipIndex{CLIP_UNDEFINED};
};


/// Holds indices to SpriteResources frames.
/// This is used to animate a sprite by calling individual frames in sequence
struct SpriteClip {
    std::vector<size_t> frames;
    bool loop{true};
};



#endif//PETIT_PROGRAM_COMPONENT_SPRITE_H
