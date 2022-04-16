#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/world/camera.hpp"
#include "engine/render/world/world.hpp"

#include <result.hpp>
#include <utility>

namespace engine::parse::xml {

auto parse_world(char const* xml_filepath) noexcept
    -> cpp::result<
        std::pair<render::world, render::camera>,
        parse_err
    >;

} // namespace engine::parse::xml
