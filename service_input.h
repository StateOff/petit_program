//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_SERVICE_INPUT_H
#define PETIT_PROGRAM_SERVICE_INPUT_H

class InputService
{
public:
    InputService()
    : m_currentKeysDown()
    , m_lastKeysDown()
    {
        std::fill(m_currentKeysDown.begin(), m_currentKeysDown.end(), false);
    }

    bool isKeyJustPressed(SDL_Scancode key)
    {
        return !m_lastKeysDown[key] && m_currentKeysDown[key];
    }

    bool isKeyJustReleased(SDL_Scancode key)
    {
        return m_lastKeysDown[key] && !m_currentKeysDown[key];
    }

    bool isKeyPressed(SDL_Scancode key)
    {
        return m_currentKeysDown[key];
    }

    void flush()
    {
        m_lastKeysDown = m_currentKeysDown;
    }

    void pushKeyDown(SDL_Scancode key)
    {
        m_currentKeysDown[key] = true;
    }

    void pushKeyUp(SDL_Scancode key)
    {
        m_currentKeysDown[key] = false;
    }

private:
    std::array<bool, SDL_NUM_SCANCODES> m_currentKeysDown;
    std::array<bool, SDL_NUM_SCANCODES> m_lastKeysDown;
};


#endif//PETIT_PROGRAM_SERVICE_INPUT_H
