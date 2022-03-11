#pragma once

#include "engine/parse/xml/err.hpp"
#include "engine/render/group/group.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_group(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::group, parse_err>;

} // namespace engine::parse::xml
