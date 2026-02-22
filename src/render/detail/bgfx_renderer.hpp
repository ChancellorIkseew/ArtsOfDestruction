#pragma once
#include <BGFX/bgfx.h>
#include <stdexcept>
#include <span>
#include "../renderer.hpp"
#include "bgfx_shader.hpp"
#include "camera/camera.hpp"
#include "math/math.hpp"

struct Vertex {
    float x, y, z;
    uint32_t color;
    float u, v;
    float nx, ny, nz;
};

class BGFXRenderer {
    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;
    bgfx::VertexLayout vertexLayout;
    bgfx::UniformHandle samplerTexColor;
    bgfx::TextureHandle texture;
public:
    BGFXRenderer(void* window, void* displayType, const IPoint2D size) {
        bgfx::Init init;
        init.platformData.nwh = window;
        init.platformData.ndt = displayType;

        init.resolution.width = static_cast<uint32_t>(size.x);
        init.resolution.height = static_cast<uint32_t>(size.y);
        init.resolution.reset = BGFX_RESET_VSYNC; // Включение VSync

        if (!bgfx::init(init))
            throw std::runtime_error("Could not initialize BGFX");

        vertexLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();

        texture = loadTextureWithSDL("res/images/icon.png");
        samplerTexColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
        bgfx::ShaderHandle vertexShader = loadShader("res/shaders/vs_prism_tex_and_light.bin");
        bgfx::ShaderHandle fragmentShader = loadShader("res/shaders/fs_prism_tex_and_light.bin");
        program = bgfx::createProgram(vertexShader, fragmentShader, true);

        onResize(size);
    }
    ~BGFXRenderer() {
        if (bgfx::isValid(program))
            bgfx::destroy(program);
        bgfx::shutdown();
    }

    void clear() {
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::touch(0);
    }
    void render(const Camera& camera, const FPoint2D windowSize) {
        const FMatrix4x4 view = camera.getView();
        const FMatrix4x4 projection = camera.getProjection(windowSize);
        bgfx::setViewTransform(0, &view, &projection);
        bgfx::frame();
    }

    void onResize(const IPoint2D size) {
        bgfx::reset(size.x, size.y, BGFX_RESET_VSYNC);
        bgfx::setViewRect(0, 0, 0, size.x, size.y);
    }

    void drawGeometry(const std::span<Vertex> vertices, const std::span<uint16_t> indices) const {
        if (vertices.empty() || indices.empty())
            return;

        bgfx::TransientVertexBuffer vertexBuffer;
        bgfx::TransientIndexBuffer indexBuffer;

        const uint32_t vertexCount = static_cast<uint32_t>(vertices.size());
        const uint32_t indexCount = static_cast<uint32_t>(indices.size());

        if (!bgfx::allocTransientBuffers(&vertexBuffer, vertexLayout, vertexCount, &indexBuffer, indexCount))
            return;

        std::memcpy(vertexBuffer.data, vertices.data(), vertexCount * sizeof(Vertex));
        std::memcpy(indexBuffer.data, indices.data(), indexCount * sizeof(uint16_t));

        bgfx::setTexture(0, samplerTexColor, texture);
        bgfx::setVertexBuffer(0, &vertexBuffer);
        bgfx::setIndexBuffer(&indexBuffer);
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, program);
    }
};
