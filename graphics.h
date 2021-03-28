//
// Created by bfloch on 3/19/21.
//

#ifndef PETIT_PROGRAM_GRAPHICS_H
#define PETIT_PROGRAM_GRAPHICS_H

#include <array>
#include <cassert>
#include <memory>
#include <vector>

#include <SDL.h>

#include "resources.h"


/// Maps colors from a uchar to an array of SDL_Colors. This makes the sprite
/// definition easier.
///
/// \param colorCode Can be any ascii character beyond the firstEntryOffset of ' ' (32)
/// \param palette Array of colors, first index being the firstEntryOffset of ' ' (32)
/// \return Color in the palette
static SDL_Color mapColor(unsigned char colorCode, const std::array<SDL_Color, 5>& palette)
{
    constexpr unsigned char firstEntryOffset = ' ';

    assert(colorCode >= firstEntryOffset && colorCode - firstEntryOffset < palette.size());

    return palette[colorCode - firstEntryOffset];
}


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
    if (pixelFormat != SDL_PIXELFORMAT_UNKNOWN)
    {
        return pixelFormat;
    }

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    assert(info.num_texture_formats > 0);
    pixelFormat = info.texture_formats[0];

    return pixelFormat;
}


/// Calculate the pitch for a format / width
/// Copied from SDL source code.
///
/// \param format
/// \param width
/// \return pitch
Sint64 calculatePitch(Uint32 format, int width)
{
    Sint64 pitch;

    if (SDL_ISPIXELFORMAT_FOURCC(format) || SDL_BITSPERPIXEL(format) >= 8)
    {
        pitch = ((Sint64) width * SDL_BYTESPERPIXEL(format));
    }
    else
    {
        pitch = (((Sint64) width * SDL_BITSPERPIXEL(format)) + 7) / 8;
    }
    pitch = (pitch + 3) & ~3; /* 4-byte aligning for speed */
    return pitch;
}
#endif//PETIT_PROGRAM_GRAPHICS_H
