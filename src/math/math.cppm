module;
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtc/type_ptr.hpp>
export module Math;

export constexpr float  PI_F = 3.1415927f;
export constexpr double PI_D = 3.141592653589793;
export constexpr float  HALF_PI_F = PI_F / 2.0f;
export constexpr double HAFL_PI_D = PI_D / 2.0;

export namespace glm {
    using vec2i = ivec2;
    using vec2f = vec2;
    using vec3f = vec3;
    using vec4f = vec4;
    using mat4f = mat4;
    using quatf = quat;
}

export using vec2i = glm::ivec2;
export using vec2f = glm::vec2;
export using vec3f = glm::vec3;
export using vec4f = glm::vec4;
export using mat4f = glm::mat4;

export namespace glm {
    using ::glm::translate;
    using ::glm::rotate;
    using ::glm::scale;
    using ::glm::perspectiveLH_ZO;
    using ::glm::orthoLH_ZO;
    using ::glm::lookAtLH;
    using ::glm::transpose;

    using ::glm::normalize;
    using ::glm::cross;
    using ::glm::dot;
    using ::glm::length;
    using ::glm::distance;
    using ::glm::lerp;
    using ::glm::clamp;
    using ::glm::radians;

    using ::glm::value_ptr;
    using ::glm::make_vec2;
    using ::glm::make_vec3;
    using ::glm::make_mat4;

    using ::glm::inverse;
    using ::glm::conjugate;
    using ::glm::slerp;
    using ::glm::mat4_cast;

    using ::glm::operator+;
    using ::glm::operator-;
    using ::glm::operator*;
    using ::glm::operator/;
}
