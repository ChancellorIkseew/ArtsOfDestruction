#include "player_controller.hpp"
//
#include <algorithm>
#include "camera/camera.hpp"
#include "math/rem_glm_convert.hpp"
#include "window/input/input.hpp"

using PlCtr = PlayerController;

void PlCtr::update(Camera& camera, const Input& input, float deltaTimeNs) const {
    // 1. Обработка поворота (Мышь)
    FPoint2D mouseDelta = input.getMouseMove();
    FPoint2D currentRot = camera.getRotation();

    currentRot.x += mouseDelta.x * lookSensitivity * deltaTimeNs;
    currentRot.y += mouseDelta.y * lookSensitivity * deltaTimeNs;

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

        FPoint3D offset = toRem(direction * moveSpeed * deltaTimeNs);

        FPoint3D position = camera.getPosition();
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
        camera.setPosition(position);
    }
}
