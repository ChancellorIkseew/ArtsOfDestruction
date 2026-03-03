module;
#include <BGFX/bgfx.h>
#include <SDL3/SDL_surface.h>
#include <cassert>
export module Textures;

import std;
import IO;
import Logger;

namespace fs = std::filesystem;
static debug::Logger logger("textures");

static bgfx::TextureHandle loadTexture(const fs::path& path) {
    const auto buffer = IO::readBinFile(path);
    SDL_IOStream* io = SDL_IOFromConstMem(buffer.data(), buffer.size());
    SDL_Surface* surface = SDL_LoadPNG_IO(io, true);
    if (!surface)
        return BGFX_INVALID_HANDLE;

    SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    if (!converted)
        return BGFX_INVALID_HANDLE;

    const bgfx::Memory* mem = bgfx::copy(converted->pixels, converted->pitch * converted->h);

    bgfx::TextureHandle handle = bgfx::createTexture2D(
        (uint16_t)converted->w,
        (uint16_t)converted->h,
        false, 1,
        bgfx::TextureFormat::RGBA8,
        BGFX_TEXTURE_NONE,
        mem
    );

    SDL_DestroySurface(converted);
    return handle;
}

static bgfx::TextureHandle buildAtlas() {

}

export class Texture {
    bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;
public:
    Texture() {
        texture = loadTexture("res/images/icon.png");
    }

    ~Texture() {
        if (bgfx::isValid(texture))
            bgfx::destroy(texture);
    }

    bgfx::TextureHandle getTexture() const {;
        return texture;
    }
private:
    void loadLayer(uint16_t index, const fs::path& path) {
        auto layer = loadTexture(path);
        const uint16_t WIDTH = 32, HEIGHT = 32;


        //const bgfx::Memory* mem = bgfx::copy(layer.idx);

        //bgfx::updateTexture2D(texture, index, 0, 0, 0, WIDTH, HEIGHT, mem);



        //SDL_DestroySurface(converted);

    }
};


