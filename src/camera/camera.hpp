#pragma once
#include "math/math.hpp"
// temporary glm includes // TODO: refactoring
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

class Camera {
private:
    FPoint3D position;
    FPoint2D rotation;
    float fov;
public:
    Camera(const FPoint3D position, const FPoint2D rotation, const float fov) :
        position(position), rotation(rotation), fov(fov) {}

    void setPosition(FPoint3D position) {
        this->position = position;
    }
    void setRotation(const FPoint2D rotation) {
        this->rotation = rotation;
    }

    FPoint3D getPosition() const { return position; }
    FPoint2D getRotation() const { return rotation; }

    FMatrix4x4 getView() const;
    FMatrix4x4 getProjection(const FPoint2D windowSize) const;

    glm::vec3 getForward() const {
        glm::quat orientation = glm::quat(glm::vec3(-rotation.y, -rotation.x, 0.0f));
        return orientation * glm::vec3(0.0f, 0.0f, 1.0f); // Для Left-Handed системы
    }

    glm::vec3 getRight() const {
        return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), getForward()));
    }
};
