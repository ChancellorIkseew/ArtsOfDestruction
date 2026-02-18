#pragma once
#include <memory>
#include "window/window.hpp"

class Renderer {
public:
    class Impl;
private:
    std::unique_ptr<Impl> pimpl;
public:
    Renderer(void* nativeWindow, void* nativeDisplayType, const FPoint2D size);
    ~Renderer();

    // Запрещаем копирование, так как рендер владеет ресурсами
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void onResize(const FPoint2D size);

    // Начало и конец кадра
    void beginFrame();
    void endFrame();

    // Пример высокоуровневого метода
    void drawPrism(const struct PrismData& data); 
};
