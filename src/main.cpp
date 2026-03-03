import std;
import Assets;
import Camera;
import DemoScene;
import Logger;
import Math;
import PlayerController;
import Renderer;
import Shaders;
import Textures;
import Window;

static debug::Logger logger("main");

int main() {
    debug::Logger::init("latest_log.txt");
    Window window({ 1280, 720 }, WindowMode::windowed);
    NativeHandle handle = window.getNativeHandle();
    Renderer renderer(handle.window, handle.displayType ,window.getSize());
    Shaders shaders;
    Texture texture;
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
        renderer.drawGeometry(vertices, indices, shaders.getShaderProgram(Shader::diffuse), texture.getTexture());
        renderer.render(camera, fWindowSize);
        window.makeFrameDelay();
    }

    return 0;
}
