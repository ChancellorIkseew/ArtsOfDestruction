#pragma once
#include <GLM/gtc/type_ptr.hpp>
#include "config.hpp"
#include "math.hpp"

rem_finline glm::mat4x4 toGlm(const FMatrix4x4& rem) {
    return glm::make_mat4x4(rem.data);
}
rem_finline_cxpr glm::vec3 toGlm(const FPoint3D rem) {
    return { rem.x, rem.y, rem.z };
}
rem_finline_cxpr glm::vec2 toGlm(const FPoint2D rem) {
    return { rem.x, rem.y };
}

rem_finline_cxpr FMatrix4x4 toRem(const glm::mat4x4 &glm) {
    return *(FMatrix4x4*)(&glm);
}

