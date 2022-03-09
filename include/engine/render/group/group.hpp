#pragma once

#include "engine/render/group/model.hpp"
#include "engine/render/group/transform.hpp"

#include <vector>

namespace engine::render {

struct group {
    std::vector<transform> transforms;
    std::vector<model> models;
    std::vector<group> children;
};

} // namespace engine::render
