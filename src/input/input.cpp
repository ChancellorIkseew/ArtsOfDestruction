#include "input.hpp"
#include "detail/sdl_input.hpp"

Input::~Input() = default;
Input::Input(std::array<Binding, BINDNAMES_COUNT> controls) : bindings(controls) {

}

void Input::update() {
    reset();
    impl->update(bindings);
}

void Input::reset() {
    lastBindingUsed = std::nullopt;
    for (auto& binding : bindings) {
        binding.justTriggered = false;
    }
}
