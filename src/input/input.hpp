#pragma once
#include <array>
#include <memory>
#include <optional>
#include "binding.hpp"

class SDLInput;

class Input {
    std::array<Binding, BINDNAMES_COUNT> bindings;
    std::optional<Binding> lastBindingUsed;

    std::unique_ptr<SDLInput> impl;
public:
    Input(std::array<Binding, BINDNAMES_COUNT> controls);
    ~Input();

    void update();

    bool active(BindName bindName) const {
        return bindings[size_t(bindName)].active;
    }
    bool jactive(BindName bindName) const {
        return bindings[size_t(bindName)].justTriggered;
    }
    std::optional<Binding> getLastBindingUsed() const {
        return lastBindingUsed;
    }
private:
    void reset();
};
