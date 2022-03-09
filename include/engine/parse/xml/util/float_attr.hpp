#pragma once

#include "engine/parse/xml/err.hpp"

#include <rapidxml.hpp>
#include <result.hpp>
#include <string_view>

namespace engine::parse::xml {

auto parse_float_attr(
    rapidxml::xml_node<> const* node,
    std::string_view attr_name
) noexcept
    -> cpp::result<float, parse_err>;

} // engine::parse::xml
