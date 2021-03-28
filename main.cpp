#include "component_sprite.h"
#include "constants.h"
#include "game.h"
#include "graphics.h"
#include "service_locator.h"
#include <SDL.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>


/// Creates a renderTarget. That is a special texture that can be used as destination
/// to render into. For example you can render into a smaller texture (backbuffer) and then
/// scale it into the window.
///
/// \param renderer
/// \param width
/// \param height
/// \return
SDL_Texture* createRendertarget(SDL_Renderer* renderer, int width, int height)
{
    Uint32 pixelFormat = getPrefferedPixelformat(renderer);

    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             pixelFormat,
                                             SDL_TEXTUREACCESS_TARGET,
                                             width,
                                             height);

    return texture;
}

#ifdef PETIT_TESTS
int main()
{
    int result = !testServiceLocator();
    printf("Result: %d\n", result);
    return result;
}
#else
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error: SDL_Init (%s)\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("Error: SDL_CreateWindow (%s)\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        printf("Error: SDL_CreateRenderer (%s)\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* renderTarget = createRendertarget(renderer, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);

    SDL_Event e;
    bool quit = false;

    Uint32 time = SDL_GetTicks();
    constexpr Uint32 targetFrameTime = 1000 / 60;

    Game game(renderer);

    InputService inputService;
    GameServiceLocator::set(&inputService);

    while (!quit)
    {
        INPUT.flush();

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EventType::SDL_KEYDOWN)
            {
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    quit = true;
                }

                INPUT.pushKeyDown(e.key.keysym.scancode);
            }
            else if (e.type == SDL_EventType::SDL_KEYUP)
            {
                INPUT.pushKeyUp(e.key.keysym.scancode);
            }
            else if (e.type == SDL_EventType::SDL_QUIT)
            {
                quit = true;
            }
        }

        if(quit) break;

        // -- Fixed timestep. Compare:
        // -- https://gafferongames.com/post/fix_your_timestep/

        Uint32 newTime = SDL_GetTicks();
        Uint32 timePassed = newTime - time;

        // Eliminate outlier by skipping frames
        timePassed = std::min(timePassed, 250u);

        while(timePassed >= targetFrameTime)
        {
            timePassed -= targetFrameTime;
            game.update(targetFrameTime);
        }
        time = SDL_GetTicks() - timePassed;

        SDL_SetRenderTarget(renderer, renderTarget);
        SDL_RenderClear(renderer);

        game.render();

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, renderTarget, nullptr, nullptr);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
#endif