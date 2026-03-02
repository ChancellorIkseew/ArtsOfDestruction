module;
export module Binding;
import std;
using std::uint8_t;
using std::size_t;

export enum class BindName : uint8_t {
    Move_left,
    Move_right,
    Move_forward,
    Move_back,
    Move_up,
    Move_down,
    //
    //Rotate_left,
    //Rotate_right,
    //Rotate_up,
    //Rotate_down,
    //
    Count
};

constexpr size_t BINDNAMES_COUNT = static_cast<size_t>(BindName::Count);

export enum class InputType : uint8_t {
    keyboard,
    mouse,
};

export enum class MouseWheelScroll : uint8_t {
    none,
    up,
    down
};

export struct Binding {
    int code;
    InputType inputType;
    mutable bool active = false;
    mutable bool justTriggered = false;

    constexpr Binding(const int code, const InputType inputType) :
        code(code), inputType(inputType) {}
    Binding() = default;
};

export class Controls {
    Binding bindings[BINDNAMES_COUNT];
public:
    Binding at(BindName bindName) const {
        return bindings[static_cast<size_t>(bindName)];
    }
    void setBinding(BindName bindName, Binding binding) {
        bindings[static_cast<size_t>(bindName)] = binding;
    }

    Binding* begin() { return bindings; }
    Binding* end() { return begin() + BINDNAMES_COUNT; }
};
