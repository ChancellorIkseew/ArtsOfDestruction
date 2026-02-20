#include "input.hpp"
//
#include <SDL3/SDL_events.h>

Input::Input() {
    controls.setBinding(BindName::Move_forward, Binding(SDL_SCANCODE_W, InputType::keyboard));
    controls.setBinding(BindName::Move_back,    Binding(SDL_SCANCODE_S, InputType::keyboard));
    controls.setBinding(BindName::Move_left,    Binding(SDL_SCANCODE_A, InputType::keyboard));
    controls.setBinding(BindName::Move_right,   Binding(SDL_SCANCODE_D, InputType::keyboard));
    controls.setBinding(BindName::Move_up,      Binding(SDL_SCANCODE_SPACE, InputType::keyboard));
    controls.setBinding(BindName::Move_down,    Binding(SDL_SCANCODE_LSHIFT, InputType::keyboard));
}

void Input::update(const SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        if (event.wheel.y < 0.0f)
            mouseWheelScroll = MouseWheelScroll::up;
        else if (event.wheel.y > 0.0f)
            mouseWheelScroll = MouseWheelScroll::down;
        return;
    }
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        mousePosition = { event.motion.x, event.motion.y };
        mouseMove = { event.motion.xrel, event.motion.yrel };
        return;
    }
    if (event.type == SDL_EVENT_TEXT_INPUT) {
        //symbolJustEntered = utf8::to_char32_t(event.text.text);
        return;
    }

    InputType inputType = InputType::keyboard;
    int code = -1;
    bool pressed = false;

    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
        code = event.key.scancode;
        pressed = event.key.down;
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        code = event.button.button;
        pressed = event.button.down;
        inputType = InputType::mouse;
    }
    else {
        return;
    }

    for (auto& binding : controls) {
        if (inputType != binding.inputType || code != binding.code)
            continue;
        binding.justTriggered = !binding.active && pressed;
        binding.active = pressed;
    }
}

void Input::reset() {
    mouseWheelScroll = MouseWheelScroll::none;
    mouseMove = { 0.f, 0.f };
    for (auto& binding : controls) {
        binding.justTriggered = false;
    }
}
