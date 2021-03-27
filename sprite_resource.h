//
// Created by bfloch on 3/19/21.
//

#ifndef PETIT_PROGRAM_SPRITE_RESOURCE_H
#define PETIT_PROGRAM_SPRITE_RESOURCE_H

/// Holds a texture atlas and rect towards if frames
class SpriteResource
{
public:
    // Make it a unique resource that can't be copied. Instead share a reference to it so that the
    // data is not duplicated
    SpriteResource() = delete;
    SpriteResource(SpriteResource&) = delete;
    SpriteResource(SpriteResource&&) = delete;
    SpriteResource& operator=(const SpriteResource&) = delete;

    /// Create a SpriteResource directly from byte data representation.
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
    SpriteResource(SDL_Renderer* renderer, const unsigned char* data, int width, int height, const std::array<SDL_Color, 5>& palette, int frames = 1)
    {
        Uint32 pixelFormat = getPrefferedPixelformat(renderer);

        m_texture = SDL_CreateTexture(renderer,
                                    pixelFormat,
                                    SDL_TEXTUREACCESS_STATIC,
                                    width,
                                    height * frames);

        SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

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
            SDL_Color color = mapColor(data[i], palette);

            // Converts the color to the expected 32 bit representation. Some formats expect
            // ABGR order, while others are just RGBA, etc.
            Uint32 v = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
            buffer[i] = v;
        }

        // The pitch is the length of each line with pixels. At the end of the line there might
        // be some bytes that are left empty (padding) so it is important to get the correct pitch.
        int pitch = calculatePitch(pixelFormat, width);

        SDL_UpdateTexture(m_texture, nullptr, buffer, pitch);

        // Data is not needed anymore since it is already uploaded to the texture (possibly GPU)
        delete[] buffer;

        // Set the rects for each frame in the sprite

        m_frames = frames;
        m_rect = new SDL_Rect[frames];
        for (int f = 0; f < frames; ++f)
        {
            m_rect[f] = SDL_Rect{0, f * height, width, height};
        }
    }

    ~SpriteResource()
    {
        if (m_rect)
        {
            delete[] m_rect;
            m_rect = nullptr;
        }
    }

    SDL_Rect& rect(int index)
    {
        assert(index < m_frames);
        return m_rect[index];
    }

    SDL_Texture* texture()
    {
        return m_texture;
    }

    [[nodiscard]] int frames() const
    {
        return m_frames;
    }

    void draw(SDL_Renderer* renderer, int frame, int x, int y, SDL_RendererFlip flip)
    {
        assert(frame < m_frames);
        SDL_Rect destRect = rect(frame);
        destRect.x = x;
        destRect.y = y;
        SDL_RenderCopyEx(renderer, m_texture, &rect(frame), &destRect, 0, nullptr, flip);
    }

private:
    SDL_Texture* m_texture;
    SDL_Rect* m_rect;
    int m_frames;
};


#endif//PETIT_PROGRAM_SPRITE_RESOURCE_H
