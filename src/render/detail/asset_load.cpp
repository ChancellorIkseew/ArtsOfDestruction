#pragma once
#include "bgfx_shader.hpp"
//
#include <fstream>
#include <SDL3/SDL_surface.h>
#include "debug/logger.hpp"

static debug::Logger logger("shader_load");

static std::string readFile(const fs::path& path) {
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        logger.error() << "File does not exist: " << fs::absolute(path);
        return std::string();
    }
    //
    std::ifstream fin(path, std::ios::binary | std::ios::ate);
    if (!fin.is_open()) {
        logger.error() << "Failed to open file: " << fs::absolute(path);
        return std::string();
    }
    //
    std::streamsize size = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::string str;
    str.resize(size);
    fin.read(str.data(), size);
    return str;
}

bgfx::ShaderHandle loadShader(const fs::path& path) {
    const std::string str = readFile(path);
    const bgfx::Memory* mem = bgfx::copy(str.data(), static_cast<uint32_t>(str.size()));
    bgfx::ShaderHandle shader = bgfx::createShader(mem);
    if (!bgfx::isValid(shader))
        logger.error() << "Shader corrupted: " << path.filename();
    return shader;
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
