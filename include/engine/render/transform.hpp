#ifndef ENGINE_RENDER_TRANSFORM_HPP
#define ENGINE_RENDER_TRANSFORM_HPP

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace engine::render {

union transform {
    enum class type {
        translate,
        rotate,
        scale
    } type;

    glm::vec3 translate;
    glm::vec4 rotate;
    glm::vec3 scale;
};

} // namespace engine::render

#endif // ENGINE_RENDER_TRANSFORM_HPP
