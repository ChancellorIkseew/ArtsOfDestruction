module;
#include <BGFX/bgfx.h>
export module Renderer;

import std;
import Assets;
import Camera;
import Math;

export struct Vertex {
    float x, y, z;
    uint32_t color;
    float u, v;
    float nx, ny, nz;
};

export struct ModelData {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
};

export class Renderer {
    bgfx::VertexLayout vertexLayout;
    bgfx::UniformHandle samplerTexColor;
public:
    Renderer(void* window, void* displayType, const vec2i size) {
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

        samplerTexColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
        onResize(size);
    }
    ~Renderer() {
        bgfx::shutdown();
    }

    void clear() {
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::touch(0);
    }
    void render(const Camera& camera, const vec2f windowSize) {
        const mat4f view = camera.getView();
        const mat4f projection = camera.getProjection(windowSize);
        bgfx::setViewTransform(0, &view, &projection);
        bgfx::frame();
    }

    void onResize(const vec2i size) {
        bgfx::reset(size.x, size.y, BGFX_RESET_VSYNC);
        bgfx::setViewRect(0, 0, 0, size.x, size.y);
    }

    void drawGeometry(const std::span<const Vertex> vertices, const std::span<const uint16_t> indices,
        bgfx::ProgramHandle shader, bgfx::TextureHandle texture) const {
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
        bgfx::submit(0, shader);
    }

    void drawGeometryI(const std::span<Vertex> vertices,
        const std::span<uint16_t> indices,
        const std::span<mat4f> instanceTransforms,
        bgfx::ProgramHandle shader,
        bgfx::TextureHandle texture) const {

        if (vertices.empty() || indices.empty() || instanceTransforms.empty())
            return;

        bgfx::TransientVertexBuffer vertexBuffer;
        bgfx::TransientIndexBuffer indexBuffer;
        bgfx::InstanceDataBuffer instanceBuffer;

        const uint32_t vertexCount = static_cast<uint32_t>(vertices.size());
        const uint32_t indexCount = static_cast<uint32_t>(indices.size());
        uint32_t instanceCount = static_cast<uint32_t>(instanceTransforms.size());

        if (!bgfx::allocTransientBuffers(&vertexBuffer, vertexLayout, vertexCount, &indexBuffer, indexCount))
            return;
        bgfx::allocInstanceDataBuffer(&instanceBuffer, instanceCount, sizeof(mat4f));

        std::memcpy(vertexBuffer.data, vertices.data(), vertexCount * sizeof(Vertex));
        std::memcpy(indexBuffer.data, indices.data(), indexCount * sizeof(uint16_t));
        std::memcpy(instanceBuffer.data, instanceTransforms.data(), instanceCount * sizeof(mat4f));

        bgfx::setTexture(0, samplerTexColor, texture);
        bgfx::setVertexBuffer(0, &vertexBuffer);
        bgfx::setIndexBuffer(&indexBuffer);
        bgfx::setInstanceDataBuffer(&instanceBuffer);
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, shader);
    }
};
