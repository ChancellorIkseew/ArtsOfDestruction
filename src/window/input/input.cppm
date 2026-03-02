module;
#include "config.hpp"
union SDL_Event;
export module Input;
import Binding;
import Math;

export class Input {
    Controls controls;
    vec2f mousePosition, mouseMove;
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

    vec2f getMousePosition() const {
        return mousePosition;
    }
    vec2f getMouseMove() const {
        return mouseMove;
    }
    MouseWheelScroll getMouseWheelScroll() const {
        return mouseWheelScroll;
    }
private:
    rem_disable_copy_and_move(Input)
};
