#include "window/window.hpp"
#include <vector>
#include "render/detail/bgfx_renderer.hpp"
#include "debug/logger.hpp"
#include "player/player_controller.hpp"
#include "demo_scene.hpp"



int main() {
    debug::Logger::init("latest_log.txt");
    Window window({ 1280, 720 }, WindowMode::windowed);
    NativeHandle handle = window.getNativeHandle();
    BGFXRenderer renderer(handle.window, handle.displayType ,window.getSize());
    const Input& input = window.getInput();
    PlayerController playerController;
    
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    buildPrism(vertices, indices, 0.0f, 0.0f, 0.0f, 1.0f);
    buildFloor(vertices, indices, 100.0f, 1.0f);

    FPoint3D position{ 5.0f, 5.0f, -10.0f };
    FPoint2D rotation{ 0.0f, 0.0f };
    const float fow = 60.0f;
    Camera camera(position, rotation, fow);
    
    window.showCursor(false);
    while (window.isOpen()) {
        window.pollEvents();
        playerController.update(camera, input, window.getRealFrameDelayNS());

        //position.z += 0.01f;
        //rotation.x += 0.01f;
        //camera.setPosition(position);
        //camera.setRotation(rotation);

        const IPoint2D iWindowSize = window.getSize();
        const FPoint2D fWindowSize{ iWindowSize.x, iWindowSize.y };

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
