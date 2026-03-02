module;
#include "config.hpp"
struct SDL_Window;
export module Window;
import std;
import Input;
import Math;
using std::uint64_t;

export enum class WindowMode : std::uint8_t { windowed, borderless, fullscreen };
export struct NativeHandle {
    void* window = nullptr;
    void* displayType = nullptr;
};

export class Window {
    SDL_Window* sdlWindow = nullptr;
    Input input;
    vec2i size;
    WindowMode mode;
    bool open = true, justResized = true, cursorVisible = true;
    uint64_t fps = 60, requiredDelayNs = 1'000'000'000 / 60, realDelayNs = 0, frameStartNs = 0;
public:
    Window(vec2i size, WindowMode mode);
    ~Window();
    //
    vec2i getSize() const { return size; }
    WindowMode getMode() const { return mode; }
    uint64_t getFPS() const { return fps; }
    bool isOpen() const { return open; }
    bool isCursorVisible() const { return cursorVisible; }
    //
    void setSize(vec2i size);
    void setMode(WindowMode mode);
    void setFPS(uint64_t fps);
    void close() { open = false; }
    void showCursor(bool flag);
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
