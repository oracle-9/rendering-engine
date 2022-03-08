#ifndef ENGINE_RENDER_CAMERA_HPP
#define ENGINE_RENDER_CAMERA_HPP

#include <glm/vec3.hpp>

namespace engine::render {

struct camera {
    glm::vec3 pos;
    glm::vec3 lookat;
    glm::vec3 up;
    glm::vec3 projection;
};
} // namespace engine::render

#endif // ENGINE_RENDER_CAMERA_HPP
