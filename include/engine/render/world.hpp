#pragma once

#include "engine/render/camera.hpp"
#include "engine/render/group/group.hpp"

namespace engine::render {

struct world {
    struct camera camera;
    group root;
};

} // namespace engine::render
