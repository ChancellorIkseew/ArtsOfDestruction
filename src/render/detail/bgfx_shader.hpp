#pragma once
#include <BGFX/bgfx.h>
#include <filesystem>
#include <fstream>

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
