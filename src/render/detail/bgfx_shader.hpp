#pragma once
#include <BGFX/bgfx.h>
#include <filesystem>
#include <fstream>
#include <SDL3/SDL_surface.h>

namespace fs = std::filesystem;

bgfx::ShaderHandle loadShader(const fs::path& path) {
    std::ifstream fin(path, std::ios::binary | std::ios::ate);
    if (!fin.is_open()) {
        // В реальном проекте здесь должен быть логгер
        return BGFX_INVALID_HANDLE;
    }

    std::streamsize size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    const bgfx::Memory* mem = bgfx::alloc(static_cast<uint32_t>(size));

    if (!fin.read(reinterpret_cast<char*>(mem->data), size))
        return BGFX_INVALID_HANDLE;
    return bgfx::createShader(mem);
}

bgfx::TextureHandle loadTextureWithSDL(const fs::path& path) {
    // 1. Загружаем через SDL3
    SDL_Surface* surface = SDL_LoadPNG(path.string().c_str());
    if (!surface)
        return BGFX_INVALID_HANDLE;

    // 2. Конвертируем в RGBA32, если формат другой (важно для предсказуемости)
    SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    if (!converted)
        return BGFX_INVALID_HANDLE;

    // 3. Копируем данные в память, которую bgfx отправит на GPU
    const bgfx::Memory* mem = bgfx::copy(converted->pixels, converted->pitch * converted->h);

    // 4. Создаем текстуру
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
