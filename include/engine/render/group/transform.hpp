#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace engine::render {

struct transform {
    enum class type {
        translate,
        rotate,
        scale
    } type;

    union {
        glm::vec3 translate;
        glm::vec4 rotate;
        glm::vec3 scale;
    };
};

} // namespace engine::render
