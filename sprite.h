//
// Created by bfloch on 3/15/21.
//

#ifndef PETIT_PROGRAM_SPRITE_H
#define PETIT_PROGRAM_SPRITE_H

#include "resources.h"
#include <SDL.h>
#include <cassert>
#include <vector>
constexpr int CLIP_UNDEFINED = -1;

/// Holds a texture atlas and rect towards if frames
struct SpriteResource {
    SDL_Texture* texture;
    SDL_Rect* rect;
    int frames;

    ~SpriteResource() {
        if(rect)
        {
            delete[] rect;
            rect = nullptr;
        }
    }
};

/// Holds indices to SpriteResources frames.
/// This is used to animate a sprite by calling individual frames in sequence
struct SpriteClip
{
    std::vector<size_t> frames;
    bool loop {true};
};

struct Sprite {
    SpriteResource* spriteResource {nullptr};
    int frame {0};
    int x {0};
    int y {0};
    int clipIndex {CLIP_UNDEFINED};
};


/// For optimal performance the pixel format of the renderer should be used.
/// That defines in which order pixels are stored, e.g. ABGR or RGBA, etc.
///
/// \param renderer
/// \return
Uint32 getPrefferedPixelformat(SDL_Renderer* renderer)
{
    // Cache the pixelformat, so we only need to get it once.
    // Note: This will not work if multiple renderers are used.

    static Uint32 pixelFormat = SDL_PIXELFORMAT_UNKNOWN;
    if(pixelFormat != SDL_PIXELFORMAT_UNKNOWN) {
        return pixelFormat;
    }

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    assert(info.num_texture_formats > 0);
    pixelFormat = info.texture_formats[0];

    return pixelFormat;
}


/// Calculate the pitch for a format / width
/// Copied from SDL source code
///
/// \param format
/// \param width
/// \return pitch
static Sint64
calculatePitch(Uint32 format, int width)
{
    Sint64 pitch;

    if (SDL_ISPIXELFORMAT_FOURCC(format) || SDL_BITSPERPIXEL(format) >= 8) {
        pitch = ((Sint64)width * SDL_BYTESPERPIXEL(format));
    } else {
        pitch = (((Sint64)width * SDL_BITSPERPIXEL(format)) + 7) / 8;
    }
    pitch = (pitch + 3) & ~3;   /* 4-byte aligning for speed */
    return pitch;
}


/// Create a Sprite directly from byte data representation.
/// Each byte must map to a Color as defined in the mapColor function.
/// Supports animation when the data consists of multiple frames. Width and height of each frame
/// must match.
///
/// \param renderer
/// \param data Array of bytes that map to Color for all frames
/// \param width Width of a single frame
/// \param height Height of a single frame
/// \param frames Number of frames in the animation
/// \return
SpriteResource createSpriteResource(SDL_Renderer* renderer, const unsigned char* data, int width, int height, int frames = 1)
{
    Uint32 pixelFormat = getPrefferedPixelformat(renderer);

    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             pixelFormat,
                                             SDL_TEXTUREACCESS_STATIC,
                                             width,
                                             height * frames);


    // The sprite will act as an atlas with all frames stored vertically.
    // [] <- First frame
    // [] <- Second frame
    // [] <- ...
    // This storage might change in future, but the sprite will correct the right
    // rect for each frame.

    int size = width * height * frames;

    // Map the bytes to proper colors within the buffer

    Uint32* buffer = new Uint32[size];
    SDL_PixelFormat* format = SDL_AllocFormat(pixelFormat);
    for (int i = 0; i < size; ++i)
    {
        SDL_Color color = mapColor(data[i]);

        // Converts the color to the expected 32 bit representation. Some formats expect
        // ABGR order, while others are just RGBA, etc.
        Uint32 v = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
        buffer[i] = v;
    }

    // The pitch is the length of each line with pixels. At the end of the line there might
    // be some bytes that are left empty (padding) so it is important to get the correct pitch.
    int pitch = calculatePitch(pixelFormat, width);

    SDL_UpdateTexture(texture, nullptr, buffer, pitch);

    // Data is not needed anymore since it is already uploaded to the texture (possibly GPU)
    delete[] buffer;

    SpriteResource sprite;
    sprite.texture = texture;

    // Set the rects for each frame in the sprite

    sprite.frames = frames;
    sprite.rect = new SDL_Rect[frames];
    for(int f = 0; f < frames; ++f)
    {
        sprite.rect[f] = SDL_Rect {0, f * height, width, height};
    }

    return sprite;
}


#endif//PETIT_PROGRAM_SPRITE_H
