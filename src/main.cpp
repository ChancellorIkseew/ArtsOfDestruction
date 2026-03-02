import std;
import Assets;
import Camera;
import DemoScene;
import Logger;
import Math;
import PlayerController;
import Renderer;
import Window;

static debug::Logger logger("main");

int main() {
    debug::Logger::init("latest_log.txt");
    Window window({ 1280, 720 }, WindowMode::windowed);
    NativeHandle handle = window.getNativeHandle();
    Renderer renderer(handle.window, handle.displayType ,window.getSize());
    const Input& input = window.getInput();
    PlayerController playerController;
    
    std::vector<Vertex> vertices;
    std::vector<std::uint16_t> indices;

    ModelData model = loadOBJ("res/models/test_town.obj");
    vertices.insert(vertices.end(), model.vertices.begin(), model.vertices.end());
    indices.insert(indices.end(), model.indices.begin(), model.indices.end());
    buildPrism(vertices, indices, 0.0f, 0.0f, 0.0f, 1.0f);
    buildFloor(vertices, indices, 100.0f, 1.0f);

    vec3f position{ 5.0f, 5.0f, -10.0f };
    vec2f rotation{ 0.0f, 0.0f };
    const float fow = 60.0f;
    Camera camera(position, rotation, fow);


    std::vector<mat4f> instanceMatrices;
    std::uint32_t gridSide = 10;
    float spacing = 2.0f;
    /*
    for (uint32_t z = 0; z < gridSide; ++z) {
        for (uint32_t x = 0; x < gridSide; ++x) {
            // Создаем базовую матрицу (Identity)
            mat4f model = mat4f(1.0f);

            // 1. Позиция (перемещаем куст в точку на сетке)
            model = glm::translate(model, vec3f(x * spacing, 0.0f, z * spacing));

            // 2. Поворот (необязательно, но добавим немного рандома для живости)
            // Если куст плоский, можно вращать только вокруг оси Y
            float randomRotation = (float)(rand() % 360);
            model = glm::rotate(model, glm::radians(randomRotation), vec3f(0.0f, 1.0f, 0.0f));

            // 3. Масштаб (чуть-чуть варьируем размер)
            float scale = 0.8f + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.4f;
            model = glm::scale(model, vec3f(scale));

            //instanceMatrices.push_back(toRem(glm::transpose(model)));
        }
    }
    */
    window.showCursor(false);
    while (window.isOpen()) {
        window.pollEvents();
        playerController.update(camera, input, window.getRealFrameDelayNS());

        //position.z += 0.01f;
        //rotation.x += 0.01f;
        //camera.setPosition(position);
        //camera.setRotation(rotation);

        const vec2i iWindowSize = window.getSize();
        const vec2f fWindowSize{ iWindowSize.x, iWindowSize.y };

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
