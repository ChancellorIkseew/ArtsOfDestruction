#pragma once
#include "math/math.hpp"

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

    FMatrix4x4 getView() const;
    FMatrix4x4 getProjection(const FPoint2D windowSize) const;
};