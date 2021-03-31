#include <iostream>
#include <vector>

#include <SDL.h>

#include "constants.h"
#include "game.h"
#include "graphics.h"
#include "service_locator.h"


#ifdef PETIT_TESTS
int main(int argc, char *argv[])
{
    int result = !testServiceLocator();
    printf("Result: %d\n", result);
    return result;
}
#else
bool handleEvents(SDL_Window* window)
{
    SDL_Event event;
    bool quit = false;
    INPUT.flush();

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                quit = true;
            }

            INPUT.pushKeyDown(event.key.keysym.scancode);
        }
        else if (event.type == SDL_KEYUP)
        {
            INPUT.pushKeyUp(event.key.keysym.scancode);

            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && event.key.keysym.mod & KMOD_ALT)
            {
                Uint32 flags = SDL_GetWindowFlags(window);
                const bool isFullscreen = flags & SDL_WINDOW_FULLSCREEN_DESKTOP || flags & SDL_WINDOW_FULLSCREEN;
                if(isFullscreen) {
                    SDL_SetWindowFullscreen(window, 0);
                }
                else {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
            }
        }
        else if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    if(event.window.data1 < BACKBUFFER_WIDTH || event.window.data2 < BACKBUFFER_HEIGHT)
                    {
                        SDL_SetWindowSize(window, std::max(BACKBUFFER_WIDTH, event.window.data1), std::max(BACKBUFFER_HEIGHT, event.window.data2));
                    }
                    break;
            }
        }
    }
    return quit;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error: SDL_Init (%s)\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
        SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        printf("Error: SDL_CreateWindow (%s)\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowMinimumSize(window, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);

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

    bool quit = false;

    Uint32 time = SDL_GetTicks();
    constexpr Uint32 targetFrameTime = 1000 / 60;

    Game game(renderer);

    InputService inputService;
    GameServiceLocator::set(&inputService);

    const double bufferAspect = BACKBUFFER_WIDTH / (double)BACKBUFFER_HEIGHT;
    while (!quit)
    {

        // -- Fixed timestep. Compare:
        // -- https://gafferongames.com/post/fix_your_timestep/

        Uint32 newTime = SDL_GetTicks();
        Uint32 timePassed = newTime - time;

        // Eliminate outlier by skipping frames
        timePassed = std::min(timePassed, 250u);

        while (timePassed >= targetFrameTime)
        {
            quit = handleEvents(window);
            if (quit) break;

            timePassed -= targetFrameTime;
            game.update(targetFrameTime);
        }
        time = SDL_GetTicks() - timePassed;

        if (quit) break;

        SDL_SetRenderTarget(renderer, renderTarget);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        game.render();


        SDL_SetRenderTarget(renderer, nullptr);

        SDL_SetRenderDrawColor(renderer, 25, 20, 33, 255);
        SDL_RenderClear(renderer);

        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        double windowAspect = windowWidth / (double)windowHeight;
        SDL_Rect targetRect;
        targetRect.w = windowWidth;
        targetRect.h = windowHeight;

        if(windowAspect > bufferAspect) {
            // Scale based on height
            int scaleFactor =  std::max(1.0, windowHeight / (double)BACKBUFFER_HEIGHT);
            targetRect.w = scaleFactor * BACKBUFFER_WIDTH;
            targetRect.h = scaleFactor * BACKBUFFER_HEIGHT;
        } else {
            // Scale based on width
            int scaleFactor =  std::max(1.0, windowWidth / (double)BACKBUFFER_WIDTH);
            targetRect.w = scaleFactor * BACKBUFFER_WIDTH;
            targetRect.h = scaleFactor * BACKBUFFER_HEIGHT;
        }

        targetRect.x = (windowWidth - targetRect.w) / 2;
        targetRect.y = (windowHeight - targetRect.h) / 2;

            SDL_RenderCopy(renderer, renderTarget, nullptr, &targetRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
#endif