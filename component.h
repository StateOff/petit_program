//
// Created by bfloch on 3/27/21.
//

#ifndef PETIT_PROGRAM_COMPONENT_H
#define PETIT_PROGRAM_COMPONENT_H

#include <SDL.h>

class Component
{
public:
    virtual ~Component() = default;

    virtual void update(Uint32 deltaTime)
    {

    }

    virtual void render(SDL_Renderer* renderer)
    {

    }

private:
};
#include "graphics.h"
#include "resources.h"
#include "sprite_animation.h"
#include "sprite_resource.h"
#include <SDL.h>
#include <cassert>
#include <memory>
#include <vector>
#endif//PETIT_PROGRAM_COMPONENT_H
