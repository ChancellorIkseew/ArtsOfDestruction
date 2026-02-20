#include "window.hpp"
//
#include <SDL3/SDL.h>
#include <stdexcept>

Window::Window(IPoint2D size, WindowMode mode) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("Could not initialize SDL");
    sdlWindow = SDL_CreateWindow("AoD demo", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!sdlWindow) {
        SDL_Quit();
        throw std::runtime_error("Could not create window");
    }

    setMode(mode);
    setSize(size);
}

Window::~Window() {
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void Window::setSize(IPoint2D size) {
    this->size = size;
    if (mode == WindowMode::windowed)
        SDL_SetWindowSize(sdlWindow, size.x, size.y);
}

void Window::setMode(WindowMode mode) {
    switch (mode) {
    case WindowMode::windowed:
        SDL_SetWindowFullscreen(sdlWindow, false);
        SDL_SetWindowBordered(sdlWindow, true);
        SDL_SetWindowSize(sdlWindow, size.x, size.y);
        SDL_SetWindowPosition(sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        break;
    case WindowMode::borderless: {
        SDL_SetWindowFullscreen(sdlWindow, false);
        SDL_SetWindowBordered(sdlWindow, false);
        const SDL_DisplayID displayID = SDL_GetDisplayForWindow(sdlWindow);
        const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(displayID);
        if (displayMode)
            SDL_SetWindowSize(sdlWindow, displayMode->w, displayMode->h);
        SDL_SetWindowPosition(sdlWindow, 0, 0);
        break;
    }
    case WindowMode::fullscreen:
        SDL_SetWindowFullscreen(sdlWindow, true);
        break;
    }
    this->mode = mode;
}

void Window::setFPS(const uint64_t fps) {
    this->fps = fps;
    constexpr uint64_t NS_PER_SECOND = 1'000'000'000;
    requiredDelayNs = NS_PER_SECOND / fps;
}

void Window::makeFrameDelay() {
    const uint64_t frameTimeNs = SDL_GetTicksNS() - frameStartNs;
    if (frameTimeNs < requiredDelayNs)
        SDL_DelayNS(requiredDelayNs - frameTimeNs);
    realDelayNs = std::max(frameTimeNs, requiredDelayNs);
    frameStartNs += realDelayNs;
}

void Window::pollEvents() {
    justResized = false;
    input.reset();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            open = false;
            break;
        case SDL_EVENT_WINDOW_RESIZED: {
            justResized = true;
            SDL_GetWindowSize(sdlWindow, &size.x, &size.y);
            break;
        }
        default:
            input.update(event);
            break;
        }
    }
}



///@brief Native OS-dependent handle.
NativeHandle Window::getNativeHandle() const {
    NativeHandle handle;
    SDL_PropertiesID id = SDL_GetWindowProperties(sdlWindow);
#if defined(_WIN32)
    handle.window = SDL_GetPointerProperty(id, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
    handle.displayType = nullptr; // Windows does not need it.

#elif defined(__APPLE__)
    handle.window = SDL_GetPointerProperty(id, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    handle.displayType = nullptr; // MacOS does not need it.

#elif defined(__linux__)
    void* display = SDL_GetPointerProperty(id, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
    if (display/*X11*/) {
        handle.displayType = display;
        handle.window = (void*)(uintptr_t)SDL_GetNumberProperty(id, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    }
    else /*Wayland*/ {
        handle.displayType = SDL_GetPointerProperty(id, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
        handle.window = SDL_GetPointerProperty(id, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
    }
#endif
    return handle;
}
