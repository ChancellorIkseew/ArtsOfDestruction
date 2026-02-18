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
    Window window({ 1280, 720 }, Window::Mode::windowed);
    Window::NativeHandle handle = window.getNativeHandle();
    BGFXRenderer renderer(handle.window, handle.displayType ,window.getSize());
    
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    buildPrism(vertices, indices, 0.0f, 0.0f, 0.0f, 1.0f);
    
    float time = 0.0f;

    while (window.isOpen()) {
        window.pollEvents();

        if (window.isJustResized()) {
            renderer.onResize(window.getSize());
        }

        renderer.clear();

        float view[16];
        float proj[16];

        float model[16];
        float result[16];

        time += 0.01f;

        // Камера: смотрим из (5, 5, -10) в центр (0, 0, 0)
        bx::Vec3 eye = { 5.0f, 5.0f, -10.0f };
        bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
        bx::mtxLookAt(view, eye, at);

        bx::mtxRotateXYZ(model, time * 0.5f, time, 0.0f);


        const FPoint2D size = window.getSize();

        bx::mtxLookAt(view, eye, at);
        bx::mtxProj(proj, 60.0f, size.x / size.y, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        // 2. Матрица конкретного объекта (вращение самой призмы)
        bx::mtxRotateXYZ(model, time * 0.5f, time, 0.0f);

        // 3. ПРИМЕНЯЕМ трансформацию к следующему вызову отрисовки
        bgfx::setTransform(model);



        renderer.drawGeometry(vertices, indices);
        renderer.render();
    }

    return 0;
}