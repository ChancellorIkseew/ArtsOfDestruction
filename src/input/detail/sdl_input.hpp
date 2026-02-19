#pragma once
#include <SDL3/SDL_events.h>
#include <span>
#include "../binding.hpp"

class SDLInput {

public:

    void update(std::span<Binding> bindings) {

        SDL_Event event = {};//

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

        //lastKeyPressed = Binding(code, inputType);

        for (auto& binding : bindings) {
            if (inputType != binding.inputType || code != binding.code)
                continue;
            binding.justTriggered = !binding.active && pressed;
            binding.active = pressed;
        }
    }
};