#include <BGFX/bgfx.h>
#include "window/window.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

struct PosColorVertex {
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    // Статический метод для инициализации разметки
    static void init() {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true) // true = нормализовать (0-255 -> 0.0-1.0)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

// Определяем статическое поле где-нибудь в .cpp
bgfx::VertexLayout PosColorVertex::ms_layout;

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

int main() {
    // 1. Инициализация окна SDL
    Window window({ 1280, 720 }, Window::Mode::windowed);

    Window::NativeHandle handle = window.getNativeHandle();
    bgfx::Init init;
    init.platformData.nwh = handle.window;
    init.platformData.ndt = handle.displayType;

    init.resolution.width = (uint32_t)window.getSize().x;
    init.resolution.height = (uint32_t)window.getSize().y;
    init.resolution.reset = BGFX_RESET_VSYNC; // Включение VSync

    // 3. Запуск BGFX
    if (!bgfx::init(init)) {
        fprintf(stderr, "Error: Could not initialize BGFX\n");
        return 1;
    }

    // 4. Настраиваем View (камеру и область отрисовки)
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0); // Серый фон
    bgfx::setViewRect(0, 0, 0, init.resolution.width, init.resolution.height);

    // 5. Инициализация Vertex Layout
    PosColorVertex::init();

    // 6. Данные треугольника (в Normalized Device Coordinates: -1.0 до 1.0)
    static PosColorVertex s_vertices[] = {
        { -0.5f, -0.5f, 0.0f, 0xff0000ff }, // Красный (Bottom-Left)
        {  0.5f, -0.5f, 0.0f, 0xff00ff00 }, // Зеленый (Bottom-Right)
        {  0.0f,  0.5f, 0.0f, 0xffff0000 }, // Синий (Top-Middle)
    };
    static const uint16_t s_indices[] = { 0, 1, 2 };

    // 7. Создаем статические буферы BGFX
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_vertices, sizeof(s_vertices)),
        PosColorVertex::ms_layout
    );
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_indices, sizeof(s_indices))
    );

    // 8. Загружаем и создаем шейдерную программу
    bgfx::ShaderHandle vertexShader = loadShader("res/shaders/vs_simple.bin");
    bgfx::ShaderHandle fragmentShader = loadShader("res/shaders/fs_simple.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vertexShader, fragmentShader, true);

    // Проверка на ошибки загрузки шейдеров
    if (!bgfx::isValid(vertexShader) || !bgfx::isValid(fragmentShader) || !bgfx::isValid(program)) {
        fprintf(stderr, "Error: Shader compilation or linking failed.\n");
        bgfx::shutdown();
        return 1;
    }

    // 9. Основной цикл отрисовки
    while (window.isOpen()) {
        window.pollEvents();

        // Обработка изменения размера окна
        if (window.isJustResized()) {
            bgfx::reset((uint32_t)window.getSize().x, (uint32_t)window.getSize().y, BGFX_RESET_VSYNC);
            bgfx::setViewRect(0, 0, 0, (uint16_t)window.getSize().x, (uint16_t)window.getSize().y);
        }

        bgfx::touch(0); // Очистить View 0

        // 10. Устанавливаем буферы и состояние
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        bgfx::setState(BGFX_STATE_DEFAULT); // Стандартное состояние (глубина, запись цвета)

        // 11. Отправляем команду отрисовки
        bgfx::submit(0, program);

        bgfx::frame(); // Завершаем кадр и обмениваем буферы
    }

    // 12. Очистка ресурсов BGFX
    bgfx::destroy(ibh);
    bgfx::destroy(vbh);
    bgfx::destroy(program);
    bgfx::shutdown();

    return 0;
}