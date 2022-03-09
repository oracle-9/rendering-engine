#ifndef ENGINE_RENDER_GROUP_HPP
#define ENGINE_RENDER_GROUP_HPP

#include "engine/render/model.hpp"
#include "engine/render/transform.hpp"

#include <vector>

namespace engine::render {

struct group {
    std::vector<transform> transforms;
    std::vector<model> models;
    std::vector<group> children;
};

} // namespace engine::render

#endif // ENGINE_RENDER_GROUP_HPP
