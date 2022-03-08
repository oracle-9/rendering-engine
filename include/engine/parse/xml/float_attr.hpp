#ifndef ENGINE_PARSE_XML_FLOAT_ATTR_HPP
#define ENGINE_PARSE_XML_FLOAT_ATTR_HPP

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

#endif // ENGINE_PARSE_XML_FLOAT_ATTR_HPP
