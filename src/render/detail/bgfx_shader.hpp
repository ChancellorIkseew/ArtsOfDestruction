#pragma once
#include <BGFX/bgfx.h>
#include <SDL3/SDL_surface.h>
#include <filesystem>

namespace fs = std::filesystem;

bgfx::ShaderHandle loadShader(const fs::path& path);
bgfx::TextureHandle loadTextureWithSDL(const fs::path& path);
