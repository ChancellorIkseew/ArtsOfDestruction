#pragma once
#include <cstdint>
#include <memory>
#include "config.hpp"
#include "math/math.hpp"

class SDLWindow;

class Window {
public:
    enum class Mode : uint8_t { windowed, borderless, fullscreen };
    struct NativeHandle {
        void* window = nullptr;
        void* displayType = nullptr;
    };
private:
    std::unique_ptr<SDLWindow> window;
    FPoint2D size;
    Mode mode;
    bool open = true, justResized = true;
public:
    Window(const FPoint2D size, const Window::Mode mode);
    ~Window();
    //
    FPoint2D getSize() const { return size; }
    Mode getMode() const { return mode; }
    bool isOpen() const { return open; }
    bool isJustResized() const { return justResized; }
    //
    void setSize(const FPoint2D size);
    void setMode(const Mode mode);
    void close();
    //
    // TODO: непарные методы будут здесь
    void pollEvents();
    //
    NativeHandle getNativeHandle();
private:
    rem_disable_copy_and_move(Window)
};
