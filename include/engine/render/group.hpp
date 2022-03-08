#ifndef ENGINE_RENDER_GROUP_HPP
#define ENGINE_RENDER_GROUP_HPP

#include "engine/render/model.hpp"
#include "engine/render/transform.hpp"

#include <vector>

struct group {
    std::vector<transform> transforms;
    std::vector<model> models;
    std::vector<group> children;
};

#endif // ENGINE_RENDER_GROUP_HPP
