#include "camera.hpp"
//
#include "math/rem_glm_convert.hpp"

FMatrix4x4 Camera::getView() const {
    glm::quat orientation = glm::quat(glm::vec3(rotation.y, rotation.x, 0.0f));
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), toGlm(position)) * glm::mat4_cast(orientation);
    glm::mat4 view = glm::inverse(transform);
    return toRem(view);
}

FMatrix4x4 Camera::getProjection(const FPoint2D windowSize) const {
    const float aspect = windowSize.x / windowSize.y;
    return toRem(glm::perspectiveLH(glm::radians(fov), aspect, 0.1f, 1000.0f));
}
