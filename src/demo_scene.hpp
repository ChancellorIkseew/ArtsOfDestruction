#pragma once
#include "render/detail/bgfx_renderer.hpp"

void buildPrism(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, float x, float y, float z, float size) {
    float height = size * 2.0f;
    uint32_t color = 0xffffffff;

    struct Pt { float x, z; };
    Pt p[] = { {0.0f, size}, {-size, -size}, {size, -size} };

    for (int i = 0; i < 3; ++i) {
        int next = (i + 1) % 3;
        uint16_t startIdx = (uint16_t)vertices.size();

        // Вычисляем нормаль боковой грани
        // 1. Берем вектор грани на плоскости XZ: (x2-x1, z2-z1)
        float dx = p[next].x - p[i].x;
        float dz = p[next].z - p[i].z;
        // 2. Получаем перпендикуляр (нормаль): (-dz, dx)
        float nx = -dz;
        float nz = dx;
        // 3. Нормализуем (приводим длину к 1.0)
        float len = std::sqrt(nx * nx + nz * nz);
        nx /= len; nz /= len;

        // Добавляем вершины грани с нормалью (nx, 0, nz)
        vertices.push_back({ x + p[i].x,    y,        z + p[i].z,    color, 0.0f, 0.0f, nx, 0.0f, nz });
        vertices.push_back({ x + p[next].x, y,        z + p[next].z, color, 1.0f, 0.0f, nx, 0.0f, nz });
        vertices.push_back({ x + p[next].x, y + height, z + p[next].z, color, 1.0f, 1.0f, nx, 0.0f, nz });
        vertices.push_back({ x + p[i].x,    y + height, z + p[i].z,    color, 0.0f, 1.0f, nx, 0.0f, nz });

        indices.push_back(startIdx + 0); indices.push_back(startIdx + 1); indices.push_back(startIdx + 2);
        indices.push_back(startIdx + 0); indices.push_back(startIdx + 2); indices.push_back(startIdx + 3);
    }

    // Крышки (упрощенно: нормаль вверх для верхней, вниз для нижней)
    // Верхняя (нормаль 0, 1, 0)
    uint16_t topStart = (uint16_t)vertices.size();
    vertices.push_back({ x + p[0].x, y + height, z + p[0].z, color, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f });
    vertices.push_back({ x + p[1].x, y + height, z + p[1].z, color, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f });
    vertices.push_back({ x + p[2].x, y + height, z + p[2].z, color, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f });
    indices.push_back(topStart); indices.push_back(topStart + 1); indices.push_back(topStart + 2);
}



void buildFloor(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, float size, float tiling) {
    uint16_t startIdx = static_cast<uint16_t>(vertices.size());

    // Нормаль смотрит строго вверх (0, 1, 0)
    // Координаты UV выходят далеко за пределы [0, 1] для тайлинга
    vertices.push_back({ -size, 0.0f,  size, 0xffffffff, 0.0f,   tiling, 0.0f, 1.0f, 0.0f });
    vertices.push_back({ size, 0.0f,  size, 0xffffffff, tiling, tiling, 0.0f, 1.0f, 0.0f });
    vertices.push_back({ size, 0.0f, -size, 0xffffffff, tiling, 0.0f,   0.0f, 1.0f, 0.0f });
    vertices.push_back({ -size, 0.0f, -size, 0xffffffff, 0.0f,   0.0f,   0.0f, 1.0f, 0.0f });

    indices.push_back(startIdx + 0);
    indices.push_back(startIdx + 2);
    indices.push_back(startIdx + 1);
    indices.push_back(startIdx + 0);
    indices.push_back(startIdx + 3);
    indices.push_back(startIdx + 2);
}
