#pragma once

class Camera;
class Input;

class PlayerController {
    float freeCamSpeed = 0.000'000'005f;
    float lookSensitivity = 0.000'000'001f;
public:
    void update(Camera& camera, const Input& input, float deltaTimeNs);
private:
    void updateFreeCamSpeed(const Input& input);
};
