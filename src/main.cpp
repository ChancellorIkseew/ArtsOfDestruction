#include <BGFX/bgfx.h>
#include <BX/math.h>
#include "window/window.hpp"
#include <vector>
#include "render/detail/bgfx_renderer.hpp"

void buildPrism(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, float x, float y, float z, float size) {
    float height = size * 2.0f;
    uint32_t color = 0xffffffff;

    // Определяем 3 точки основания (вид сверху)
    struct Pt { float x, z; float u; };
    Pt p[] = {
        { 0.0f,  size, 0.5f }, // перед
        { -size, -size, 0.0f }, // лево-зад
        { size,  -size, 1.0f }  // право-зад
    };

    // --- 1. БОКОВЫЕ ГРАНИ (3 грани по 4 вершины = 12 вершин) ---
    for (int i = 0; i < 3; ++i) {
        int next = (i + 1) % 3;
        uint16_t startIdx = (uint16_t)vertices.size();

        // Порядок: низ-лево, низ-право, верх-право, верх-лево
        vertices.push_back({ x + p[i].x,    y,          z + p[i].z,    color, 0.0f, 0.0f });
        vertices.push_back({ x + p[next].x, y,          z + p[next].z, color, 1.0f, 0.0f });
        vertices.push_back({ x + p[next].x, y + height, z + p[next].z, color, 1.0f, 1.0f });
        vertices.push_back({ x + p[i].x,    y + height, z + p[i].z,    color, 0.0f, 1.0f });

        // Индексы (CCW - против часовой стрелки)
        indices.push_back(startIdx + 0); indices.push_back(startIdx + 1); indices.push_back(startIdx + 2);
        indices.push_back(startIdx + 0); indices.push_back(startIdx + 2); indices.push_back(startIdx + 3);
    }

    // --- 2. НИЖНЕЕ ОСНОВАНИЕ (3 вершины) ---
    uint16_t botIdx = (uint16_t)vertices.size();
    vertices.push_back({ x + p[0].x, y, z + p[0].z, color, 0.5f, 1.0f });
    vertices.push_back({ x + p[1].x, y, z + p[1].z, color, 0.0f, 0.0f });
    vertices.push_back({ x + p[2].x, y, z + p[2].z, color, 1.0f, 0.0f });

    indices.push_back(botIdx + 0); indices.push_back(botIdx + 2); indices.push_back(botIdx + 1);

    // --- 3. ВЕРХНЕЕ ОСНОВАНИЕ (3 вершины) ---
    uint16_t topIdx = (uint16_t)vertices.size();
    vertices.push_back({ x + p[0].x, y + height, z + p[0].z, color, 0.5f, 1.0f });
    vertices.push_back({ x + p[1].x, y + height, z + p[1].z, color, 0.0f, 0.0f });
    vertices.push_back({ x + p[2].x, y + height, z + p[2].z, color, 1.0f, 0.0f });

    indices.push_back(topIdx + 0); indices.push_back(topIdx + 1); indices.push_back(topIdx + 2);
}

int main() {
    Window window({ 1280, 720 }, WindowMode::windowed);
    NativeHandle handle = window.getNativeHandle();
    BGFXRenderer renderer(handle.window, handle.displayType ,window.getSize());
    const Input& input = window.getInput();
    
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    buildPrism(vertices, indices, 0.0f, 0.0f, 0.0f, 1.0f);

    FPoint3D position{ 5.0f, 5.0f, -10.0f };
    FPoint2D rotation{ 0.0f, 0.0f };
    const float fow = 60.0f;
    Camera camera(position, rotation, fow);

    while (window.isOpen()) {
        window.pollEvents();

        const IPoint2D iWindowSize = window.getSize();
        const FPoint2D fWindowSize{ iWindowSize.x, iWindowSize.y };

        const float delta = 0.01f;
        if (input.active(BindName::Move_forward))
            position.z += delta;
        if (input.active(BindName::Move_back))
            position.z -= delta;
        if (input.active(BindName::Move_left))
            position.x += delta;
        if (input.active(BindName::Move_right))
            position.x -= delta;
        if (input.active(BindName::Move_up))
            position.y += delta;
        if (input.active(BindName::Move_down))
            position.y -= delta;

        rotation.x += input.getMouseMove().x * delta;
        rotation.y += input.getMouseMove().y * delta;

        camera.setPosition(position);
        camera.setRotation(rotation);

        if (window.isJustResized()) {
            renderer.onResize(iWindowSize);
        }

        renderer.clear();
        renderer.drawGeometry(vertices, indices);
        renderer.render(camera, fWindowSize);
        window.makeFrameDelay();
    }

    return 0;
}
