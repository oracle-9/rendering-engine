#ifndef ENGINE_RENDER_WORLD_HPP
#define ENGINE_RENDER_WORLD_HPP

#include "engine/render/camera.hpp"
#include "engine/render/group.hpp"

namespace engine::render {

struct world {
    struct camera camera;
    group root;
};

} // namespace engine::render

#endif // ENGINE_RENDER_WORLD_HPP
