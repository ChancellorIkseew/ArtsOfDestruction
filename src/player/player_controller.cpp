#include "player_controller.hpp"
//
#include <algorithm>
#include "camera/camera.hpp"
#include "math/rem_glm_convert.hpp"
#include "window/input/input.hpp"

using PlCtr = PlayerController;

void PlCtr::updateFreeCamSpeed(const Input& input) {
    constexpr float MIN_FRE_CAM_SPEED = 0.000'000'001f;
    constexpr float MAX_FRE_CAM_SPEED = 0.000'000'050f;
    constexpr float SCALE_FACTOR = 1.2f;

    switch (input.getMouseWheelScroll()) {
    case MouseWheelScroll::none:
        return;
    case MouseWheelScroll::up:
        freeCamSpeed /= SCALE_FACTOR;
        break;
    case MouseWheelScroll::down:
        freeCamSpeed *= SCALE_FACTOR;
        break;
    }
    freeCamSpeed = std::clamp(freeCamSpeed, MIN_FRE_CAM_SPEED, MAX_FRE_CAM_SPEED);
}

void PlCtr::update(Camera& camera, const Input& input, float deltaTimeNs) {
    updateFreeCamSpeed(input);
    // 1. Обработка поворота (Мышь)
    FPoint2D mouseDelta = input.getMouseMove();
    FPoint2D currentRot = camera.getRotation();

    currentRot.x -= mouseDelta.x * lookSensitivity * deltaTimeNs;
    currentRot.y -= mouseDelta.y * lookSensitivity * deltaTimeNs;

    // Ограничиваем наклон головы (Pitch), чтобы не сделать сальто
    currentRot.y = std::clamp(currentRot.y, -HALF_PI_F, HALF_PI_F);

    camera.setRotation(currentRot);

    // 2. Обработка движения (Клавиатура)
    glm::vec3 direction(0.0f);
    glm::vec3 forward = camera.getForward();
    glm::vec3 right = camera.getRight();

    if (input.active(BindName::Move_forward)) direction += forward;
    if (input.active(BindName::Move_back))    direction -= forward;
    if (input.active(BindName::Move_right))   direction += right;
    if (input.active(BindName::Move_left))    direction -= right;
    if (input.active(BindName::Move_up))      direction.y += 1.0f;
    if (input.active(BindName::Move_down))    direction.y -= 1.0f;

    // Нормализуем, чтобы по диагонали не бегать быстрее
    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction);
        FPoint3D offset = toRem(direction * freeCamSpeed * deltaTimeNs);

        FPoint3D position = camera.getPosition();
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
        camera.setPosition(position);
    }
}
