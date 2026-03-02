module;
#include <BGFX/bgfx.h>
export module Assets;
import std;

export struct ModelData;
namespace fs = std::filesystem;

export bgfx::ShaderHandle loadShader(const fs::path& path);
export bgfx::TextureHandle loadTextureWithSDL(const fs::path& path);
export ModelData loadOBJ(const fs::path& path);
