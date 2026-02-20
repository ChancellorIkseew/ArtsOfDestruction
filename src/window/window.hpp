#pragma once
#include <cstdint>
#include "config.hpp"
#include "input/input.hpp"
#include "math/math.hpp"

struct SDL_Window;
enum class WindowMode : uint8_t { windowed, borderless, fullscreen };
struct NativeHandle {
    void* window = nullptr;
    void* displayType = nullptr;
};

class Window {
    SDL_Window* sdlWindow = nullptr;
    Input input;
    IPoint2D size;
    WindowMode mode;
    bool open = true, justResized = true;
    uint64_t fps = 60, requiredDelayNs = 16, realDelayNs = 0, frameStartNs = 0;
public:
    Window(IPoint2D size, WindowMode mode);
    ~Window();
    //
    IPoint2D getSize() const { return size; }
    WindowMode getMode() const { return mode; }
    uint64_t getFPS() const { return fps; }
    bool isOpen() const { return open; }
    //
    void setSize(IPoint2D size);
    void setMode(WindowMode mode);
    void setFPS(uint64_t fps);
    void close() { open = false; }
    //
    void pollEvents();
    void makeFrameDelay();
    //
    NativeHandle getNativeHandle() const;
    uint64_t getRealFrameDelayNS() const { return realDelayNs; }
    bool isJustResized() const { return justResized; }
    Input& getInput() { return input; }
private:
    rem_disable_copy_and_move(Window)
};
