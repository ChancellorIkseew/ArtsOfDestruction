#pragma once
#include <BGFX/bgfx.h>
#include <stdexcept>
#include "../renderer.hpp"

using BGFXRenderer = Renderer::Impl;

class BGFXRenderer {
    bgfx::Init init;
public:
    BGFXRenderer(void* window, void* displayType, const FPoint2D size) {
        init.platformData.nwh = window;
        init.platformData.ndt = displayType;

        init.resolution.width = static_cast<uint32_t>(size.x);
        init.resolution.height = static_cast<uint32_t>(size.y);
        init.resolution.reset = BGFX_RESET_VSYNC; // Включение VSync

        if (!bgfx::init(init))
            throw std::runtime_error("Could not initialize BGFX");
    }
    ~BGFXRenderer() {
        bgfx::shutdown();
    }
};


