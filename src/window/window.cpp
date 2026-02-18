#include "window.hpp"
//
#include "detail/sdl_window.hpp"

// PIMPL constructor and destructor in cpp.
Window::~Window() = default;
Window::Window(const FPoint2D size, const Window::Mode mode) :
    size(size), mode(mode), window(std::make_unique<SDLWindow>(size, mode)) {

}

void Window::setSize(const FPoint2D size) {
    window->setSize(size);
    this->size = window->getSize();
}
void Window::setMode(const Mode mode) {
    window->setMode(mode);
    this->mode = window->getMode();
}
void Window::close() {
    window->close();
    open = window->isOpen();
}

void Window::pollEvents() {
    window->pollEvents();
    open = window->isOpen();
    justResized = window->isJustResized();
    size = window->getSize();
}

Window::NativeHandle Window::getNativeHandle() {
    return window->getNativeHandle();
}
