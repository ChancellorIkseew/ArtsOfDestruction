#pragma once
#include <SDL3/SDL.h>
#include <stdexcept>
#include "window/window.hpp"

using Mode = Window::Mode;
using Handle = Window::NativeHandle;

class SDLWindow {
    SDL_Window* sdlWindow;
	SDL_Event event = {};
	FPoint2D size;
	Mode mode;
	bool open = true, justResized = true;
public:
    SDLWindow(const FPoint2D size, const Window::Mode mode) : size(size), mode(mode) {
		if (!SDL_Init(SDL_INIT_VIDEO))
			throw std::runtime_error("Could not initialize SDL");
		sdlWindow = SDL_CreateWindow("AoD demo", 800, 600, SDL_WINDOW_RESIZABLE);
		if (!sdlWindow) {
			SDL_Quit();
			throw std::runtime_error("Couldn't create window");
		}	
    }

	~SDLWindow() {
		SDL_DestroyWindow(sdlWindow);
		SDL_Quit();
	}

	void setMode(const Mode mode) {
		switch (mode) {
		case Mode::windowed:
			SDL_SetWindowFullscreen(sdlWindow, false);
			SDL_SetWindowBordered(sdlWindow, true);
			SDL_SetWindowSize(sdlWindow, size.x, size.y);
			SDL_SetWindowPosition(sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			break;
		case Mode::borderless: {
			SDL_SetWindowFullscreen(sdlWindow, false);
			SDL_SetWindowBordered(sdlWindow, false);
			const SDL_DisplayID displayID = SDL_GetDisplayForWindow(sdlWindow);
			const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(displayID);
			if (displayMode)
			    SDL_SetWindowSize(sdlWindow, displayMode->w, displayMode->h);
			SDL_SetWindowPosition(sdlWindow, 0, 0);
		    break;
		}
		case Mode::fullscreen:
			SDL_SetWindowFullscreen(sdlWindow, true);
			break;
		}
		this->mode = mode;
	}

	void pollEvents() {
		justResized = false;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				open = false;
				break;
			case SDL_EVENT_WINDOW_RESIZED: {
				justResized = true;
				int w = 0, h = 0;
				SDL_GetWindowSize(sdlWindow, &w, &h);
				size = { float(w), float(h) };
			    break;
			}
			default: break;
			}
		}
	}

	void setSize(const FPoint2D size) { this->size = size; }
	void close() { open = false; }

	FPoint2D getSize() const { return size; }
	Window::Mode getMode() const { return mode; }
	bool isOpen() const { return open; }
	bool isJustResized() const { return justResized; }

	Handle getNativeHandle() const;
};


///@brief Native OS-dependent handle.
Handle SDLWindow::getNativeHandle() const {
	Handle handle;
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
