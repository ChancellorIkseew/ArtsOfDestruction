#include <BGFX/bgfx.h>
#include <BX/math.h>
#include "window/window.hpp"
#include <vector>
#include "render/detail/bgfx_renderer.hpp"

void buildPrism(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, float x, float y, float z, float size) {
    uint16_t baseIdx = (uint16_t)vertices.size();

    // 6 вершин (3 снизу, 3 сверху)
    // Основание (y)
    vertices.push_back({ x,        y, z + size, 0xff0000ff }); // 0: низ перед
    vertices.push_back({ x - size, y, z - size, 0xff00ff00 }); // 1: низ лево
    vertices.push_back({ x + size, y, z - size, 0xffff0000 }); // 2: низ право

    // Верхушка (y + высота)
    float height = size * 2.0f;
    vertices.push_back({ x,          y + height, z + size, 0xff0000ff }); // 3: верх перед
    vertices.push_back({ x - size, y + height, z - size, 0xff00ff00 }); // 4: верх лево
    vertices.push_back({ x + size, y + height, z - size, 0xffff0000 }); // 5: верх право

    // Индексы (8 треугольников = 24 индекса)
    uint16_t prismIndices[] = {
        // Основания
        0, 1, 2,       // низ
        3, 5, 4,       // верх
        // Боковые грани (по 2 треугольника на грань)
        0, 3, 4, 0, 4, 1, // грань лево-перед
        1, 4, 5, 1, 5, 2, // грань зад
        2, 5, 3, 2, 3, 0  // грань право-перед
    };

    for (auto i : prismIndices) {
        indices.push_back(baseIdx + i);
    }
}

int main() {
    Window window({ 1280, 720 }, WindowMode::windowed);
    NativeHandle handle = window.getNativeHandle();
    BGFXRenderer renderer(handle.window, handle.displayType ,window.getSize());
    
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    buildPrism(vertices, indices, 0.0f, 0.0f, 0.0f, 1.0f);

    const FPoint3D position{ 5.0f, 5.0f, -10.0f };
    const FPoint2D rotation{ 0.0f, 0.0f };
    const float fow = 60.0f;
    Camera camera(position, rotation, fow);

    while (window.isOpen()) {
        window.pollEvents();

        const IPoint2D iWindowSize = window.getSize();
        const FPoint2D fWindowSize{ iWindowSize.x, iWindowSize.y };

        if (window.isJustResized()) {
            renderer.onResize(iWindowSize);
        }

        renderer.clear();

        renderer.drawGeometry(vertices, indices);
        renderer.render(camera, fWindowSize);
    }

    return 0;
}
