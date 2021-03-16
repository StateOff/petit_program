#include "sprite.h"
#include <SDL.h>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

constexpr const char* APP_NAME = "Petit Program";
constexpr int BACKBUFFER_WIDTH = 128;
constexpr int BACKBUFFER_HEIGHT = 72;
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;


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
    SpriteResource playerSpriteResource = createSpriteResource(renderer, PLAYER, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_FRAMES);

    Sprite player;
    player.spriteResource = &playerSpriteResource;

    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EventType::SDL_KEYDOWN)
            {
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    quit = true;
                }
            }
            if (e.type == SDL_EventType::SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderTarget(renderer, renderTarget);
        SDL_RenderClear(renderer);

        SDL_Rect destRect = playerSpriteResource.rect[0];
        destRect.x = player.x;
        destRect.y = player.y;
        SDL_RenderCopy(renderer, playerSpriteResource.texture, &player.spriteResource->rect[player.frame], &destRect);

        player.frame = (player.frame + 1) % player.spriteResource->frames;
        SDL_Delay(160);

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
