module PlayerController;
import std;

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
    vec2f mouseDelta = input.getMouseMove();
    vec2f cameraRotation = camera.getRotation();

    cameraRotation -= mouseDelta * lookSensitivity * deltaTimeNs;
    cameraRotation.y = std::clamp(cameraRotation.y, -HALF_PI_F, HALF_PI_F); // clamp pitch(тангаж)
    camera.setRotation(cameraRotation);

    // 2. Обработка движения (Клавиатура)
    vec3f direction(0.0f);
    vec3f forward = camera.getForward();
    vec3f right = camera.getRight();

    if (input.active(BindName::Move_forward)) direction += forward;
    if (input.active(BindName::Move_back))    direction -= forward;
    if (input.active(BindName::Move_right))   direction += right;
    if (input.active(BindName::Move_left))    direction -= right;
    if (input.active(BindName::Move_up))      direction.y += 1.0f;
    if (input.active(BindName::Move_down))    direction.y -= 1.0f;

    // Нормализуем, чтобы по диагонали не бегать быстрее
    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction);
        vec3f offset = direction * freeCamSpeed * deltaTimeNs;

        vec3f position = camera.getPosition();
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
        camera.setPosition(position);
    }
}
