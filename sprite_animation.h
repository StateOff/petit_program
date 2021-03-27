//
// Created by bfloch on 3/19/21.
//

#ifndef PETIT_PROGRAM_SPRITE_ANIMATION_H
#define PETIT_PROGRAM_SPRITE_ANIMATION_H

#include "sprite_resource.h"
class SpriteAnimation
{
public:
    SpriteAnimation(std::shared_ptr<SpriteResource> spriteResource, std::vector<size_t>&& keyFrames)
        : m_spriteResource(std::move(spriteResource))
        , m_keyFrames(keyFrames)
    {
    }

    void
    play()
    {
        m_state = State::SPRITE_ANIMATION_PLAYING_FORWARD;
    }

    void stop()
    {
        m_state = State::SPRITE_ANIMATION_PAUSED;
        m_frame = 0;
    }

    void pause()
    {
        m_state = State::SPRITE_ANIMATION_PAUSED;
    }

    bool isPlaying()
    {
        return m_state != State::SPRITE_ANIMATION_PAUSED;
    }

    void update(Uint32 deltaTime)
    {
        switch (m_state)
        {
            case SPRITE_ANIMATION_PLAYING_FORWARD:
                m_frame += deltaTime / 1000.0 * m_speed;
                break;
            case SPRITE_ANIMATION_PAUSED: break;
        }

        while (m_frame >= frames())
        {
            m_frame = m_frame - frames();
        }
    }

    void setSpeed(double speed)
    {
        m_speed = speed;
    }

    [[nodiscard]] int frames() const
    {
        return m_keyFrames.size();
    }

    void draw(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip)
    {
        m_spriteResource->draw(renderer, m_keyFrames[m_frame], x, y, flip);
    }

    void setFrame(int frame)
    {
        m_frame = std::min(frame, frames() - 1);
    }

private:
    enum State
    {
        SPRITE_ANIMATION_PAUSED,
        SPRITE_ANIMATION_PLAYING_FORWARD,
    };

    State m_state{SPRITE_ANIMATION_PLAYING_FORWARD};
    std::shared_ptr<SpriteResource> m_spriteResource;
    std::vector<size_t> m_keyFrames;
    double m_frame{0.0};
    double m_speed{1.0};
};


#endif//PETIT_PROGRAM_SPRITE_ANIMATION_H
