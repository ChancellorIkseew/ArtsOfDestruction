#pragma once
#include "binding.hpp"
#include "config.hpp"
#include "math/math.hpp"

union SDL_Event;

class Input {
    Controls controls;
    FPoint2D mousePosition, mouseMove;
    MouseWheelScroll mouseWheelScroll = MouseWheelScroll::none;
public:
    Input();

    void update(const SDL_Event& event);
    void reset();

    bool active(BindName bindName) const {
        return controls.at(bindName).active;
    }
    bool jactive(BindName bindName) const {
        return controls.at(bindName).justTriggered;
    }

    FPoint2D getMousePosition() const {
        return mousePosition;
    }
    FPoint2D getMouseMove() const {
        return mouseMove;
    }
    MouseWheelScroll getMouseWheelScroll() const {
        return mouseWheelScroll;
    }
private:
    rem_disable_copy_and_move(Input)
};
