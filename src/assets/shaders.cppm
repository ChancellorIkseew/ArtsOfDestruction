module;
#include <BGFX/bgfx.h>
#include <cassert>
export module Shaders;

import std;
import IO;
import Logger;

namespace fs = std::filesystem;
static debug::Logger logger("shaders");

export enum class Shader {
    main,
    instancing,
    // other
    count
};

static bgfx::ShaderHandle loadShader(const fs::path& path) {
    const auto str = IO::readBinFile(path);
    const bgfx::Memory* mem = bgfx::copy(str.data(), static_cast<uint32_t>(str.size()));
    bgfx::ShaderHandle shader = bgfx::createShader(mem);
    if (!bgfx::isValid(shader))
        logger.error() << "Shader corrupted: " << path.filename();
    return shader;
}

static bgfx::ProgramHandle createShaderProgram(const fs::path& vertex, const fs::path& fragment) {
    bgfx::ShaderHandle vertexShader = loadShader(vertex);
    bgfx::ShaderHandle fragmentShader = loadShader(fragment);
    const auto program = bgfx::createProgram(vertexShader, fragmentShader, true);
    if (!bgfx::isValid(program))
        logger.error() << "Failed to create shader program from " <<
        vertex.filename().string() << " and " << fragment.filename().string();
    return program;
}

export class Shaders {
    std::array<bgfx::ProgramHandle, size_t(Shader::count)> programs;
public:
    Shaders() {
        programs.fill(BGFX_INVALID_HANDLE);
        programs[static_cast<size_t>(Shader::main)] = createShaderProgram(
            "res/shaders/vs_prism_tex_and_light.bin",
            "res/shaders/fs_prism_tex_and_light.bin"); // names will be changed.
        // other shaders.
    }

    ~Shaders() {
        for (auto& program : programs) {
            if (bgfx::isValid(program))
                bgfx::destroy(program);
        }
    }

    bgfx::ProgramHandle getShaderProgram(Shader shader) const {
        assert(bgfx::isValid(programs[static_cast<size_t>(shader)]));
        return programs[static_cast<size_t>(shader)];
    }
};
