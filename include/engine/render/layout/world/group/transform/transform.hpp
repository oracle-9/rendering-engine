#pragma once

#include "engine/render/layout/world/group/transform/rotate.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace engine::render {

struct Transform {
    enum class Kind {
        TRANSLATE,
        ROTATE,
        SCALE
    } kind;

    union {
        glm::vec3 translate;
        Rotate rotate;
        glm::vec3 scale;
    };
};

} // namespace engine::render
