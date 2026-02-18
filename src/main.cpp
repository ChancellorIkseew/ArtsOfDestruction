#include <BGFX/bgfx.h>
#include "window/window.hpp"
#include <vector>
#include "render/detail/bgfx_renderer.hpp"

int main() {
    // 1. Инициализация окна SDL
    Window window({ 1280, 720 }, Window::Mode::windowed);
    Window::NativeHandle handle = window.getNativeHandle();
    BGFXRenderer renderer(handle.window, handle.displayType ,window.getSize());

    // 6. Данные треугольника (в Normalized Device Coordinates: -1.0 до 1.0)
    std::vector<Vertex> vertexes = {
        { -0.5f, -0.5f, 0.0f, 0xff0000ff }, // Красный (Bottom-Left)
        {  0.5f, -0.5f, 0.0f, 0xff00ff00 }, // Зеленый (Bottom-Right)
        {  0.0f,  0.5f, 0.0f, 0xffff0000 }, // Синий (Top-Middle)
    };
    std::vector<uint16_t> indexes = { 0, 1, 2 };
    
    // 9. Основной цикл отрисовки
    while (window.isOpen()) {
        window.pollEvents();

        if (window.isJustResized()) {
            renderer.onResize(window.getSize());
        }

        renderer.clear();
        renderer.drawGeometry(vertexes, indexes);
        renderer.render();
    }

    return 0;
}