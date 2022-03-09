#pragma once

#include "engine/parse/xml/err.hpp"
#include "engine/render/world.hpp"

#include <result.hpp>

namespace engine::parse::xml {

auto parse_world(char const* xml_filepath) noexcept
    -> cpp::result<render::world, parse_err>;

} // namespace engine::parse::xml
