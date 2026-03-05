module;
#include <BGFX/bgfx.h>
export module Assets;

import std;
import Vertex;

namespace fs = std::filesystem;

export ModelData loadOBJ(const fs::path& path);
