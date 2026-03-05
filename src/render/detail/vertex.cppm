module;
export module Vertex;

import std;
import Math;

export struct Vertex {
    float x, y, z;
    std::uint32_t color;
    float u, v;
    float nx, ny, nz;
};

export struct ModelData {
    std::vector<Vertex> vertices;
    std::vector<std::uint16_t> indices;
};

export struct InstanceData {
    mat4f transform;
    float textureIndex;
    std::array<float, 3> padding;
};
