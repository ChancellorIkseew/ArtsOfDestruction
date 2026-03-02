module;
export module Camera;
import Math;

export class Camera {
private:
    vec3f position;
    vec2f rotation;
    float fov;
public:
    Camera(const vec3f position, const vec2f rotation, const float fov) :
        position(position), rotation(rotation), fov(fov) {}

    void setPosition(vec3f position) {
        this->position = position;
    }
    void setRotation(vec2f rotation) {
        this->rotation = rotation;
    }

    vec3f getPosition() const { return position; }
    vec2f getRotation() const { return rotation; }

    vec3f getForward() const {
        glm::quatf orientation = glm::quatf(vec3f(-rotation.y, -rotation.x, 0.0f));
        return orientation * vec3f(0.0f, 0.0f, 1.0f); // Для Left-Handed системы
    }

    vec3f getRight() const {
        return glm::normalize(glm::cross(vec3f(0.0f, 1.0f, 0.0f), getForward()));
    }

    mat4f getView() const {
        glm::quatf orientation = glm::quatf(vec3f(-rotation.y, -rotation.x, 0.0f));
        mat4f transform = glm::translate(mat4f(1.0f), position) * glm::mat4_cast(orientation);
        mat4f view = glm::inverse(transform);
        return view;
    }

    mat4f getProjection(const vec2f windowSize) const {
        const float aspect = windowSize.x / windowSize.y;
        return glm::perspectiveLH_ZO(glm::radians(fov), aspect, 0.1f, 1000.0f);
    }
};
